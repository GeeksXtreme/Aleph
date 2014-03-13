Project ALEPH 
==

Please check the issues page to see our current tasks and progress. 
----


During a [penetration test](http://en.wikipedia.org/wiki/Penetration_test), a common way to prove insecurities on a target system is by exploiting vulnerable programs on that system and cause them to open a shell program (cmd.exe on Windows, BASH on Linux and Mac OS) to which the pentester can connect, allowing them a significant amount of power over the operating system, user data, etc. Provided that the pentester maintains access to this machine without detection, the machine can then be further used to exploit other machines on the network, increasing the probability of a successful pentest. 

However, it is very easy for a smart system administrator to detect that a shell is open locally, and an intrusion detection/prevention system can be configured to block network traffic that looks like it belongs to a [reverse shell](http://en.wikipedia.org/wiki/Shellcode#Remote). One strategy for maintaining persistent covert remote access is by employing a [rootkit](http://en.wikipedia.org/wiki/Rootkit). 

Purpose
---
ALEPH is a Windows rootkit designed to be configurable based on the user's needs, incredibly powerful once installed on a target host, and very hard to detect. The purpose of this project is mainly for fun and learning, but also to design a novel security testing tool.

ALEPH is released under GNU GPL (see license.txt) and is specifically for legitimate use for penetration testers, malware analysts, and other security professionals. ALEPH is not a hacking tool and not intended for malicious use. 

"The day will not save them. And we own the night." --- Horus 
