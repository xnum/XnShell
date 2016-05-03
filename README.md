unix prog. hw3
-------------

1. 執行指令，例如`ls`，`vim` 

2. 執行一個指令並殺死他`sleep 10` and Ctrl+C

3. 重導向`ls -al > /tmp/x` `cat /tmp/x` 和 `cat < /etc/passwd`

4. 建立pipe `cat /etc/passwd | cat | less`

5. process group `ps -o pid,sid,pgid,ppid,cmd | cat | cat | tr A-Z a-z`

6. background process `python -m SimpleHTTPServer 8083 &`
 
7. Ctrl+Z 暫停process `fg`喚醒process `fg 1`喚醒特定process `lsjob`列出process

8. `quit` `exit` 離開

9. `xenv add LANG C` 增加環境變數 `xenv rm LANG` 移除環境變數


known bugs:

1. 執行一個找不到的指令 有可能造成程式hang住
