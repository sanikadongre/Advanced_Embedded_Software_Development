echo -e "userinfo">hw1_problem6.txt
who -u>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "OS Type">>hw1_problem6.txt
uname -a>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "OS Distribution"
lsb_release -a>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "OS Version"
cat /etc/os-release>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "Kernel Version"
uname -or>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "Kernel gcc version build"
cat /proc/version>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "Kernel build time"
uname -v>>hw1_problem6.txt
echo >>hw1_problem6.txt
echo -e "System Architecture Information"
uname -a>>hw1_problem6.txt
echo  >>hw1_problem6.txt
echo -e "Information on file System Memory"
free -m>>hw1_problem6.txt
echo >>hw1_problem6.txt

