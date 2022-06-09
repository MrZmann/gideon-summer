import subprocess

perfStatEvents = {"cpu-cycles","instructions","cache-references","cache-misses","branch-instructions","branch-misses","bus-cycles","L1-dcache-loads","L1-dcache-load-misses","L1-dcache-stores","LLC-loads","LLC-load-misses","LLC-stores","LLC-store-misses","dTLB-load-misses","dTLB-store-misses","iTLB-load-misses","branch-loads","branch-load-misses"}

f = open("output.csv", "a")

def profileMap(size, mapType, maxLoad):
	events = ""
	for event in perfStatEvents:
		events += event
		events += ","
	#remove extra comma at the end
	events = events[:len(events)-1]
	
	#run the command in shell and record its output
	lines = subprocess.run(["perf", "stat", "-x", ",", "-e", events, "./test", "-s", size, "-m", mapType, "-l", maxLoad, ">&", "tmp"],check=True, capture_output=True, text=True)

	#stdout will only be the output of the c++ program (perf outputs to cerr)
	#this means all of the data is good except for the trailing newline character
	output = lines.stdout[:len(lines.stdout)-1]
	
	#split the cerr results into individual lines and then take the first number from each line
	#the first number of each line is the value for each perfStatEvent
	perfResults = lines.stderr.split("\n")
	for i in perfResults:
		output += i.split(",")[0]
		output += ","
		
	#remove the trailing comma and add a newline
	output = output[:len(output)-1]
	output += "\n"
	f.write(output)


def testXTimes(size, mapType, maxLoad, num):
	for i in range(1, num+1):
		print("Profiling " + mapType + " map, size " + size + ", maxLoad " + maxLoad + " : " + str(i) + "/" + str(num))
		profileMap(size, mapType, maxLoad)
		
testXTimes("10000000","chained","2",10)
testXTimes("10000000","open","0.7",10)
testXTimes("10000000","std","2",10)
testXTimes("10000000","stdunordered","2",10)

f.close()
