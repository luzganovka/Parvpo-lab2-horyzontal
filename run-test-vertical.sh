#!/bin/bash
for cpus in 0.2 0.4 .6 .8 1 1.2 1.4 1.6 1.8 2 2.2 2.4 2.6 2.8 3;  do 
	echo -en "CPUs:\t$cpus\t"; 
	sed -i 's/cpus: .*$/cpus: "'$cpus'"/' ./docker-compose.yaml; 
	docker-compose up 2>&1 | grep -E 'consumer_1.*Time'; 
done
