unix prog. hw3
-------------

1. 執行指令，例如`ls`，`vim` 

2. 執行一個指令並殺死他`sleep 10` and Ctrl+C

3. 重導向`ls -al > /tmp/x` `cat /tmp/x` 和 `cat < /etc/passwd`

4. 建立pipe `cat /etc/passwd | cat | less`

5. process group `ps -o pid,sid,pgid,ppid,cmd | cat | cat | tr A-Z a-z`
