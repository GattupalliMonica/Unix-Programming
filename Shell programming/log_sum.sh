#!/bin/bash

input=("${@: -1}")
Usage()
{
   
   echo "Usage: $0 [-n N] (-c|-2|-r|-F|-t) <filename>"
   exit 1 
}

#Function data() stores the entire log file
data()
{
    while IFS= read -r line
    do
    echo "$line"
    done < "$input"
}

l=0

log_c()
{
#it is converted into expression and grep is used for pattern matching.
IP_Addr=$(data |grep -Eo '[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}')  
#uniq is used to count the occurances of ip address and sorted them in descending order. 
IP_Addr="`echo "$IP_Addr" | sort | uniq -c | sort -n -r`" 
#now the ip address and their occurance are printed
IP_Addr="`echo "$IP_Addr" |awk '{ print " " $2"  "$1 " " }'`"
#below if statment is used to in execute the file with or without -n in the command line
if [ $l -ne 0 ];
then 
	
	echo "$IP_Addr" | head -n $l
else
	echo "$IP_Addr"
fi

}

log_2()
{
#it is converted into expression and grep is used for pattern matching and finds number of successful connections.
IP_Success=$(data | grep "HTTP\/[1]\.[0-1]\" [2][0-9][0-9]" | grep -Eo '[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}')
#uniq is used to count the occurances of ip address and sorted them in descending order. 
IP_Addr="`echo "$IP_Success" | sort | uniq -c | sort -n -r`"
#now the ip address and their successful attempts  are printed
IP_Addr="`echo "$IP_Addr" | awk '{print " " $2"  "$1 " "}'`"
#below if statment is used to in execute the file with or without -n in the command line
if [ $l -ne 0 ];
then
	echo "$IP_Addr" | head -n $l
else
	echo "$IP_Addr"
fi


}

log_r()
{
#Argument 9 indicates status code, ip address are sorted as per status code
Result_code=$(data | awk '{print $9}' | sort |uniq -c| sort -n -r )
# Result codes are matched using grep command
Result_code=$(echo "$Result_code"| grep "[2-4][0][0-4]" | grep -Eo "[2-4][0][0-4]")
#echo "$Result_code"
#for loop is used to print all ip addresses under specific Result code
      	for code in $Result_code;
	do
                #printing status code
		echo $code
                #match all the ip addresses with specific status code
		IP_Success=$(data | grep "HTTP\/[1]\.[0-1]\" $code" | awk '{print $1,$9}')
                #match all the ip addresses with specific status code
	        result="`echo "$IP_Success"| sort | uniq -c | sort -nr |awk '{print " "$3"  "$2 "  "}'`"
                #below if statment is used to in execute the file with or without -n in the command line
		if [ $l -ne 0 ];
                then
			echo "$result" | head -n $l
		else
			echo "$result"
		fi
	done
}

log_F()
{
#matching the status code with connection
Result_code=$(data |grep  "HTTP\/[1]\.[0-1]\" [4][0][0-4]")
#storing the vals of Result code 
Result_code=$(echo "$Result_code" | awk '{print $9}')
#sorting the staus codes
Result_code="`echo "$Result_code" | sort| uniq -c| sort -nr | awk '{print $2}'`"
#for loop is used to print the status code under the specific status code
	for code in $Result_code;
	do
                #printing Result code
		echo $code
                #match all the ip addresses with specific Result code
		IP_Success=$(data | grep "HTTP\/[1]\.[0-1]\" $code" | awk '{print $1,$9}')
                #match all the ip addresses with specific status code
		result=$(echo "$IP_Success"| sort|uniq -c | sort -n -r| awk '{ print " "$3"  " $2 " "}')
		#below if statment is used to in execute the file with or without -n in the command line
                if [ $l -ne 0 ];
                then
			echo "$result" | head -n $l
		else
			echo "$result"
		fi
	done		        
}


log_t()
{
#it captures the ip address along with the number of bytes tranmitted
store_IPByte=$(data | awk '{print $1,$10}'| sort -k1,1)
#echo "$store_IPByte"
#it removes all failed connections
store_IPByte="`echo "$store_IPByte" |awk '{print $1,$2}'|grep -v "-"`"
#echo "$store_IPByte"
#it sort every ip address in decresing order
store_IP=$(echo "$store_IPByte" |awk '{print $1}'| sort | uniq |sort -nr| awk '{print $1}')
#echo "$store_IP"

	declare -A Lst_IP
#for loop is used to add up all the byte transmitations under similar ip address.
	for IP in ${store_IP[@]};
	do
		sum=0
		byte=$(echo "$store_IPByte"|grep "$IP"|awk '{print $2}' )
	
		for val in ${byte[@]};
		do
			sum=$(expr $sum + $val)
		done
		Lst_IP[$IP]=$sum 
	done
	#below if statment is used to in execute the file with or without -n in the command line
	if [ $l -ne 0 ];
        then
	
                # this for loop is used to print the ip address along with the number of byte transmissions.   
		for IP in ${!Lst_IP[@]}; 
		do
			echo -e "$IP   ${Lst_IP[$IP]} "
		done | sort -k2 -r -n | head -n $l
	else
                for IP in ${!Lst_IP[@]}; 
		do
			echo -e "$IP   ${Lst_IP[$IP]} "
		done | sort -k2 -r -n
	fi
	 

}


# below if-elif statment is used in the case of swapping the arguments in the command line
if [ $1 == "-n"  ]
then  
	
	l=$2
	shift 2
	while getopts "n:c:2:r:F:t:" opt
        do
               case "$opt" in
			
      		c ) log_c $l;;
      		2 ) log_2 $l ;;
      		r ) log_r $l ;;
      		F ) log_F $l;;
      		t ) log_t $l ;;
      		? ) Usage ;; # Print helpFunction in case parameter is non-existent
   		esac
	
	done
elif [ $2 == "-n" ]
then
	l=$3
	#shift 2
        while getopts "n:c:2:r:F:t:" opt
        do        
          	case "$opt" in
		
      		c ) log_c $l;;
      		2 ) log_2 $l ;;
      		r ) log_r $l ;;
      		F ) log_F $l ;;
      		t ) log_t $l ;;
      		? ) Usage ;; # Print helpFunction in case parameter is non-existent
   		esac
     done
else
	while getopts "n:c:2:r:F:t:" opt
        do        
          	case "$opt" in
		
      		c ) log_c $l;;
      		2 ) log_2 $l ;;
      		r ) log_r $l ;;
      		F ) log_F $l ;;
      		t ) log_t $l ;;
      		? ) Usage ;; # Print helpFunction in case parameter is non-existent
   		esac
     done
fi

