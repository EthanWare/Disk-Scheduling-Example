#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Assume no more than 1000 requests in the input file */
#define BUFSIZE 1000

/* Assume 5000 cylinders */
#define CYL 5000

/* Assume no requests will be longer than 80 characterse */
#define LINELEN 80

int fcfs(int initpos, int requests[], int nmemb) {
	//loop variable, total head movement, current head position
	int i,total,currentPos;
	currentPos = initpos;
	total = 0;
	
	//add up the difference between each request starting at the first request, ending at the last request
	for(i = 0;i < nmemb;i++){
		if(requests[i] > currentPos)
			total += requests[i] - currentPos;
		else
			total += currentPos - requests[i];
		currentPos = requests[i];
	}
	return total;
}

int sstf(int initpos, int requests[], int nmemb) {
	//loop and temp variables
	int i,j,temp;
	//current head position, request with the minimum seek time from the current head position,
	//index of the request with the minimum seek time from the current head position, current seek time from the current head position
	int currentPos,closestDistance,closestDistanceIndex,distance;
	//used to sort the requests in SSTF order
	int sortedArray[nmemb];
	
	currentPos = initpos;
	j = 0;
	
	//copy request[] into sortedArray[]
	for(i = 0;i < nmemb;i++){
		sortedArray[i] = requests[i];
	}
	
	//sort the requests in SSTF order
	for(i = 0;i < nmemb;i++){
		j = i;
		closestDistance = CYL + 1;
		//loop through all the requests
		while(j < nmemb){
			//find distance from current head position
			if(sortedArray[j] > currentPos)
				distance = sortedArray[j] - currentPos;
			else
				distance = currentPos - sortedArray[j];
			//if the distance is smaller than the current closest distance,
			//update the current closest distance and closest distance index
			if(distance < closestDistance){
				closestDistance = distance;
				closestDistanceIndex = j;
			}
			j++;
		}
		//swap the request with the minimum seek time from the current head position with the sortedArray[i]
		temp = sortedArray[i];
		sortedArray[i] = sortedArray[closestDistanceIndex];
		sortedArray[closestDistanceIndex] = temp;
		//set the current position to the request with the minimum seek time from the current head position
		currentPos = sortedArray[i];
	}
	
	//find total head movement by passing the SSTF sorted array to the FCFS function
	return fcfs(initpos,sortedArray,nmemb);
}

int scan(int initpos, int requests[], int nmemb) {
	//loop vairable, lowest request, highest request, total head movement
	int i,lowest,highest,total;
	
	total = 0;
	lowest = initpos;
	highest = initpos;
	
	//loop through the requests and find the highest and lowest
	for(i = 0;i < nmemb;i++){
		if(requests[i] < lowest)
			lowest = requests[i];
		else if(requests[i] > highest)
			highest = requests[i];
	}
	//if the highest request is greater than the starting position,
	//total head movement is from initpos to 0, and 0 to highest request
	if(highest > initpos)
		total = initpos + highest;
	//else the total head movement is from initpos to the lowest request
	else
		total = initpos - lowest;
	
	return total;
}

int cscan(int initpos, int requests[], int nmemb) {
	//loop vairable, closest request to initpos, highest request, total head movement
	int i,closest,highest,total;
	
	total = 0;
	closest = -1;
	highest = initpos;
	
	//loop through the requests and find the highest and lowest,
	//as well as the closest to the starting position
	for(i = 0;i < nmemb;i++){
		if(requests[i] < initpos){
			if(requests[i] > closest)
				closest = requests[i];
		}
		else if(requests[i] > highest)
			highest = requests[i];
	}
	//if the closest request is less than the starting position, total head movement is
	//from initpos to the max cylinder, the max cylinder to 0, and 0 to closest request
	if(closest < initpos)
		total = (CYL - initpos) + CYL + closest;
	//else the total head movement is from initpos to highest request
	else
		total = highest - initpos;
	
	return total;
}

int look(int initpos, int requests[], int nmemb) {
	//loop vairable, lowest request, highest request, total head movement
	int i,lowest,highest,total;
	
	total = 0;
	lowest = initpos;
	highest = initpos;
	
	//loop through the requests and find the highest and lowest
	for(i = 0;i < nmemb;i++){
		if(requests[i] < lowest)
			lowest = requests[i];
		else if(requests[i] > highest)
			highest = requests[i];
	}
	//if the highest request is greater than the starting position, total head
	//movement is from initpos to lowest request, and lowest request to highest request
	if(highest > initpos)
		total = (initpos - lowest) + (highest - lowest);
	//else the total head movement is from initpos to the lowest request
	else
		total = initpos - lowest;
	
	return total;
}

int clook(int initpos, int requests[], int nmemb) {
	//loop vairable, closest request to initpos, lowest request, highest request, total head movement
	int i,closest,lowest,highest,total;
	
	total = 0;
	closest = -1;
	highest = initpos;
	lowest = initpos;
	
	//loop through the requests and find the highest and lowest,
	//as well as the closest to the starting position
	for(i = 0;i < nmemb;i++){
		if(requests[i] < initpos){
			if(requests[i] > closest)
				closest = requests[i];
			if(requests[i] < lowest)
				lowest = requests[i];
		}
		else if(requests[i] > highest)
			highest = requests[i];
	}
	//if the closest request is less than the starting position, total head movement is
	//from initpos to highest request, highest request to lowest request, and lowest request to closest request
	if(closest < initpos)
		total = (highest - initpos) + (highest - lowest) + (closest - lowest);
	//else the total head movement is from initpos to highest request
	else
		total = highest - initpos;
	
	return total;
}

main(int argc, char* argv[])
{
	char s[LINELEN];
	int requests[BUFSIZE];
	int count;
	int initpos;

	if (argc < 2)
	{
		printf("Usage: ./a.out initpos\n");
		exit(1);
	}

	initpos = atoi(argv[1]);

	count=0;
	while (fgets(s, LINELEN, stdin))
		requests[count++]=atoi(s);

	printf("FCFS: %d\n", fcfs(initpos, requests, count));
	printf("SSTF: %d\n", sstf(initpos, requests, count));
	printf("SCAN: %d\n", scan(initpos, requests, count));
	printf("C-SCAN: %d\n", cscan(initpos, requests, count));
	printf("LOOK: %d\n", look(initpos, requests, count));
	printf("C-LOOK: %d\n", clook(initpos, requests, count));

}
