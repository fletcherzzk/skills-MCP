## What the skill does ##

The skills reviews code for type consistency, correctness, immutability, side effects, recursion, and functional programming style.

It is triggered when checking code, reviewing code and identifying optimization opportunities.



## What the MCP does ##
Retrieve Functional reference implementations and metadata for code improvement.
this tool is used whenever improving, optimizing, refactoring, or reviewing code, especially when replacing imperative loops, reducing mutation, introducing functional programming patterns, or improving parallelism.




## The friction that motivated each ## 
1. The model did not set the ignore, which made me commit a lot of unnessary files and I have to withdraw the commit.
2. The fact that the code written by AI is not always the best. From what 15122 and 15150 emphasized, code should be as in good style, which motivated me to create a skill for code style.




## How to install both ## 


## Installing the Skill ## 

Copy the code-style-check folder into the project's .agents/skills/ directory.

The resulting structure should be:

your-project/
└── .agents/
    └── skills/
        └── code-style-check/
            └── SKILL.md

The filename must be exactly SKILL.md, and the folder name must match the skill name declared in the file's YAML frontmatter.

Restart or reload VS Code after installing the Skill.

To verify installation, open a new Codex session and type $ in the prompt box. The code-style-check Skill should appear in the selection menu.

The Skill can then be invoked using $code-style-check. It generates a review report without modifying the source code being reviewed.


## Installing the MCP Server ## 

1. Requirements:

Python with a version compatible with the project's dependencies

uv for Python environment and dependency management

OpenAI Codex VS Code extension, or another MCP-compatible client

Clone this repository and navigate to the server directory:

cd path/to/skills-MCP/codecheck

Install the project's dependencies:

uv sync

The server uses the Python MCP SDK and does not require an external hosted service.

The server entry point is codecheck.py. It loads the function catalog and C++ source files from the local library/ directory.

2. Registering the MCP Server in Codex

Open the Codex configuration file:

~/.codex/config.toml

On Windows, this is typically located at:

C:/Users/<USERNAME>/.codex/config.toml

Add the following configuration:

[mcp_servers.functional-cpp-library]
command = "uv"
args = [
    "run",
    "--directory",
    "C:/path/to/skills-MCP/codecheck",
    "python",
    "codecheck.py"
]

Important: Replace C:/path/to/skills-MCP/codecheck with the absolute path to the server directory on your own machine.

Use forward slashes for Windows paths in TOML. The uv executable must also be available on the system PATH.

After updating config.toml, reload VS Code using Developer: Reload Window and start a new Codex session.

The server uses STDIO for MCP communication and does not require a network port.

3. Verifying the MCP Server

The MCP server exposes a single tool:

lookup_function(name, implementation="all")

For example:

name = "map"
implementation = "parallel_stl"

The tool returns the function's metadata and actual C++ source code from the library.

To test the tool manually, the MCP Inspector can be launched from the server directory:

npx.cmd @modelcontextprotocol/inspector uv run python codecheck.py

This requires Node.js and npm. The .cmd form is useful on Windows PowerShell installations where script execution policies prevent npx.ps1 from running.

To confirm Codex integration, ask Codex to retrieve a reference implementation and verify that its activity record contains an actual lookup_function MCP tool call.

A correct textual answer alone does not demonstrate that the tool was called.

Citation: this part is wrriten by AI, and I have checked it.








## Which tools you confirmed it works in ##

The toolkit was tested using the following tools:

1. Codex VS Code extension : Skill discovery, explicit Skill invocation, and MCP tool calls

2. MCP Inspector: Server connection, tool discovery, and manual tool invocation (Note: this is really helpful )




## AI Usage##
I used ChatGPT web version and Codex IDE during the development of this toolkit.

ChatGPT assisted with understanding the MCP architecture, planning the functional C++ reference library, troubleshooting Windows PowerShell commands, generating test code, and interpreting debugging and testing results. 

Codex assisted with implementing and revising the Python MCP server and conducting the tests.








## Step2 development trace ##
How ChatGPT and Codex use skills

ChatGPT and Codex can activate skills in two ways:

Explicit invocation: Include the skill directly in your prompt. In ChatGPT, type @ to select a skill. In Codex CLI or the IDE extension, run /skills or type $ to mention a skill.
Implicit invocation: ChatGPT or Codex can choose a skill when your task matches the skill description.

I am going to use explict invocation, to make sure the skill is used by the agent.


Step3-9: see other files



