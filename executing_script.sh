#!/bin/bash
for (( i=0; i < 3; i++ ))

do
	cd /home/sachin/os_lab1/os_Lab_TUD
	./$1 <  $1.txt
done
