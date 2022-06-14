import subprocess
import random

perfStatEvents = ["L1-dcache-loads", "L1-dcache-load-misses", "LLC-loads", "LLC-load-misses","branch-loads","branch-load-misses","cpu-cycles","instructions"]

#perfStatEvents = [["cpu-cycles","instructions","cache-references","cache-misses","branch-instructions","branch-misses","bus-cycles"],["L1-dcache-loads","L1-dcache-load-misses","L1-dcache-stores"], ["LLC-loads","LLC-load-misses"], ["LLC-stores","LLC-store-misses"], ["dTLB-load-misses","dTLB-store-misses","iTLB-load-misses","branch-loads","branch-load-misses"]]

f = open("output.csv", "a")


def getEvents(perfStatEvents):
	events = ""
	for event in perfStatEvents:
		events += event
		events += ","
	#remove extra comma at the end
	events = events[:len(events)-1]
	return events
	
def formatPerfResults(perfResults):
	#remove empty item at end
	perfResults.pop(len(perfResults)-1)
	out = ""
	for i in perfResults:
		individuals = i.split(",")
		#print(individuals)
		if(individuals[4] != "100.00"):
			print("perf not running at 100%\n")
		
		out += individuals[0]
		out += ","
	return out
	
def profileMap(size, mapType, maxLoad):
	events = getEvents(perfStatEvents)
	
	#run the command in shell and record its output
	lines = subprocess.run(["perf", "stat", "-x", ",", "-e", events, "./bench", "-s", size, "-m", mapType, "-l", maxLoad],check=True, capture_output=True, text=True)
	
	#stdout will only be the output of the c++ program (perf outputs to cerr)
	#this means all of the data is good except for the trailing newline character
	output = lines.stdout[:len(lines.stdout)-1]

	#split the cerr results into individual lines and then take the first number from each line
	#the first number of each line is the value for each perfStatEvent
	temp = formatPerfResults(lines.stderr.split("\n"))
	output += temp
	
	#this is unnecessary; with the smaller number of perf stat events, everything runs at 100% in one run
	#split to have all perf stat events run 100% of the time
	#for i in perfStatEvents:
		#events = getEvents(i)
	
		#run the command in shell and record its output
		#lines = subprocess.run(["perf", "stat", "-x", ",", "-e", events, "./test", "-s", size, "-m", mapType, "-l", maxLoad],check=True, capture_output=True, text=True)
	
		#split the cerr results into individual lines and then take the first number from each line
		#the first number of each line is the value for each perfStatEvent
		#temp = formatPerfResults(lines.stderr.split("\n"))
		#output += temp
	
	#add a newline
	output += "\n"
	f.write(output)
	print(output)


def testXTimes(size, mapType, maxLoad, num):
	for i in range(1, num+1):
		print("Profiling " + mapType + " map, size " + size + ", maxLoad " + maxLoad + " : " + str(i) + "/" + str(num))
		profileMap(size, mapType, maxLoad)
		
def testDiffLoads(size, mapType, maxLoads, num):
	for i in range (1, num+1):
		for j in range (0, len(maxLoads)):
			print("Profiling " + mapType + " map, size " + size + ", maxLoad " + maxLoads[j] + " : " + str(i) + "/" + str(num))
			profileMap(size, mapType, maxLoads[j])
			
def testIncrementalLoads(size, mapType, maxLoads):
	for j in range (0, len(maxLoads)):
		print("Profiling " + mapType + " map, size " + size + ", maxLoad " + maxLoads[j] + " : " + str(j+1) + "/" + str(len(maxLoads)))
		profileMap(size, mapType, maxLoads[j])
		
			

#maxLoadsChained = ["0.5","0.75","1","1.5","2","2.5","3","4","5","6","8","10","15","20","25","30","40"]
#maxLoadsOpen = {"0.2","0.3","0.4","0.5","0.55","0.6","0.65","0.7","0.75","0.8","0.85","0.9"}

#maxLoadsChainedInt = [0.01]
#
#while maxLoadsChainedInt[len(maxLoadsChainedInt)-1] < 0.99:
#	maxLoadsChainedInt.append(maxLoadsChainedInt[len(maxLoadsChainedInt)-1]+0.01)
#	
#maxLoadsChainedStr = []
#
#for i in maxLoadsChainedInt:
#	maxLoadsChainedStr.append(str(format(i, "0.2f")))
#
#print(maxLoadsChainedStr)
#random.shuffle(maxLoadsChainedStr)
#print("\n\n")
#print(maxLoadsChainedStr)
#
#testIncrementalLoads("10000000","open",maxLoadsChainedStr)






profilingRuns = []
#100,000 to 10,000,000
for i in range(1,101):
	profilingRuns.append([str(100000*i), "chained", "1.9"])
	profilingRuns.append([str(100000*i), "open", "0.7"])
	profilingRuns.append([str(100000*i), "std", "1"])
	profilingRuns.append([str(100000*i), "stdunordered", "1"])
print(profilingRuns)
random.shuffle(profilingRuns)
print("\n\n")
print(profilingRuns)
print(len(profilingRuns))
print("\n\n")

for i in range (0, len(profilingRuns)):
	print("Profiling " + profilingRuns[i][1] + " map, size " + profilingRuns[i][0] + ", maxLoad " + profilingRuns[i][2] + " : " + str(i+1) + "/" + str(len(profilingRuns)))
	profileMap(profilingRuns[i][0], profilingRuns[i][1], profilingRuns[i][2])

f.close()
