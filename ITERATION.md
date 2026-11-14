Problem: explict invocation works very well, but implict invocation does not work well.

My diagnosis of why: I think the skill might not be properly registered as I can check with implict prompt multiple times, it still does not work.

What you changed, and the result of rerunning it: 
 Update: fixed. Should be SKILL.md instead of skill.md (Really annoying LOL)
 As a result, the skill finally shows up in the tool bar for codex, indicating that it fully works.



Problem: AI does not use MCP without explict instruction

My diagnosis of why: I think this has some problem with MCP description docstring

What I have changed: I updated the docString
  I stressed that the MCP should be used even if standard-library solutions are known and stressed that MCP should be used when modifying C++ Code

Result: 
 With a new session, the agent uses the MCP this time, which is very good!
 ## see score_changed2.cpp ## 









