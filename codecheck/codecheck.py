"""Read-only Functional C++ library reference server using MCP over STDIO."""

import json
import logging
import sys
from pathlib import Path
from typing import Any

from mcp.server import MCPServer

logger = logging.getLogger(__name__)
mcp = MCPServer("functional-cpp-library")
LIBRARY_DIR = Path(__file__).resolve().parent / "library"


def _library_path(relative_path: str) -> Path:
    """Resolve a catalog path, rejecting absolute paths and escaping symlinks."""
    path = Path(relative_path)
    # Reject drive-relative paths and Windows alternate data streams as well.
    if not relative_path.strip() or path.anchor or ":" in relative_path or "\x00" in relative_path:
        raise ValueError("Catalog paths must be relative paths within library/.")
    resolved = (LIBRARY_DIR / path).resolve()
    if not resolved.is_relative_to(LIBRARY_DIR):
        raise ValueError("Catalog paths must stay within library/.")
    return resolved


def _load_catalog() -> dict[str, Any]:
    """Reload and validate the catalog's existing function/implementation mapping."""
    try:
        with _library_path("catalog.json").open("r", encoding="utf-8") as source:
            catalog = json.load(source)
    except FileNotFoundError:
        raise ValueError("Missing library/catalog.json.") from None
    except (json.JSONDecodeError, UnicodeError, RecursionError):
        raise ValueError("library/catalog.json is not valid UTF-8 JSON.") from None
    except (OSError, RuntimeError):
        raise ValueError("Unable to read library/catalog.json.") from None

    if not isinstance(catalog, dict) or not isinstance(catalog.get("functions"), dict):
        raise ValueError("Invalid catalog: 'functions' must be an object.")
    for name, function in catalog["functions"].items():
        if not name.strip() or not isinstance(function, dict):
            raise ValueError("Invalid catalog: each function needs a name and an object.")
        implementations = function.get("implementations")
        if not isinstance(implementations, dict) or not implementations:
            raise ValueError(f"Invalid catalog: {name!r} needs a nonempty 'implementations' object.")
        for variant, metadata in implementations.items():
            if not variant.strip() or variant == "all" or not isinstance(metadata, dict):
                raise ValueError(f"Invalid catalog: {name!r} has an invalid implementation entry.")
            if not isinstance(metadata.get("path"), str) or not metadata["path"].strip():
                raise ValueError(f"Invalid catalog: {name!r}/{variant!r} needs a source 'path'.")
    return catalog


@mcp.tool()
def lookup_function(name: str, implementation: str = "all") -> str:
    """Retrieve actual Functional C++ reference implementations and catalog metadata.

    Use this tool when an agent needs reliable reference implementations for
    functional C++ programming, including map, tabulate, reduce, filter, and sort.
    Supported function names and implementation variants come from catalog.json.
    This read-only tool never generates, compiles, or executes C++ code.

    Args:
        name: Exact function name registered in the library catalog.
        implementation: Exact catalog variant name, or "all" (the default) to
            retrieve every available implementation variant for the function.

    Returns:
        A JSON string containing library, function, and implementation metadata
        plus source_code for each requested variant, or a helpful error string.
        Source text is preserved exactly, including line endings, in the decoded
        JSON source_code values. Missing metadata is omitted; signatures and
        additional restrictions may be present in the source itself.
    """
    if not isinstance(name, str) or not name.strip():
        return "Error: name must be a nonempty string."
    if not isinstance(implementation, str) or not implementation.strip():
        return "Error: implementation must be a nonempty string or 'all'."

    try:
        catalog = _load_catalog()
    except (ValueError, OSError, RuntimeError) as error:
        logger.warning("Library catalog lookup failed.")
        # Only deliberate validation messages, never OS paths or file contents.
        if isinstance(error, ValueError):
            return f"Error: {error}"
        return "Error: Unable to access library/catalog.json."

    functions = catalog["functions"]
    if name not in functions:
        alternatives = ", ".join(sorted(functions)) or "(none)"
        return f"Error: Unknown function {name!r}. Available functions: {alternatives}."

    function = functions[name]
    variants = function["implementations"]
    if implementation != "all" and implementation not in variants:
        alternatives = ", ".join(sorted(variants))
        return (
            f"Error: Unknown implementation {implementation!r} for {name!r}. "
            f"Available implementations: {alternatives}. Use 'all' to retrieve every variant."
        )

    selected = variants if implementation == "all" else {implementation: variants[implementation]}
    implementations = []
    for variant, metadata in selected.items():
        try:
            path = _library_path(metadata["path"])
            if not path.is_file():
                raise FileNotFoundError
            # newline="" prevents Python from normalizing the original C++ text.
            with path.open("r", encoding="utf-8", newline="") as source:
                code = source.read()
        except FileNotFoundError:
            return f"Error: Source file missing or not a regular file for {name!r}/{variant!r}."
        except UnicodeError:
            return f"Error: Source file for {name!r}/{variant!r} is not valid UTF-8."
        except ValueError:
            return f"Error: Invalid source path for {name!r}/{variant!r}; it must stay within library/."
        except (OSError, RuntimeError):
            logger.warning("Library source lookup failed.")
            return f"Error: Unable to read the source file for {name!r}/{variant!r}."
        implementations.append({"name": variant, "metadata": metadata, "source_code": code})

    # Preserve all metadata, including future catalog fields, without inference.
    result = {
        "library_metadata": {key: value for key, value in catalog.items() if key != "functions"},
        "function": name,
        "function_metadata": {key: value for key, value in function.items() if key != "implementations"},
        "available_implementations": list(variants),
        "implementations": implementations,
    }
    return json.dumps(result, ensure_ascii=False, indent=2)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, stream=sys.stderr)
    mcp.run(transport="stdio")
