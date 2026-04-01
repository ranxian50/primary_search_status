#! /bin/bash


echo "请输入你的名字"
read name
channel="猜数字游戏"
haveDo=0

echo "您好，欢迎来到$channel!"
number=$(shuf -i 1-10 -n 1)
#echo $number
while true
do
echo "请输入一个一到十之间的数字"
read guess
haveDo=$((++haveDo))
if [[ $guess -eq $number ]]; then
	echo "恭喜你猜对了，一共尝试了$haveDo次，是否继续？（y/n 不区分大小写）"
	read choice
	if [[ $choice == "y" ]] || [[ $choice == "Y"]]; then
		number=$((RANDOM%10+1)) #加一是为了让随机数范围在1-10，而不是0-9
		#echo $number
		haveDo=0
		continue
	else
		break
	fi
elif [[ $guess -lt $number ]]; then
	echo "小了"
else
	echo "大了"

fi
done
