
# Parsing input file into reports 
filename = "input.txt"
reports = []
with open(filename, "r") as inputfile:
    for line in inputfile:
        reports += [ list(map(int, line.split())) ]

# Test if 2 levels respect the condition
def SafeLevel(level, next_level, increasing):
    if 1 > abs(level - next_level) or abs(level - next_level) > 3:
        return False
    next_increase = next_level - level > 0
    if next_increase != increasing:
        return False
    return True

# Reccursive function to test if all levels in the report respect the condition
# Tolerance can be 0 or 1
def IsSafeReport(report, index, tolerance, increasing):
    #Reached tolerance max
    if tolerance < 0:
        return False

    #Reached the last element
    if(index >= len(report)-1):
        return True

    current = report[index]
    next = report[index+1]
    #First case scenario
    if index==0:
        increasing = next - current > 0
        if SafeLevel(current, next, increasing):
            return IsSafeReport(report, 1, tolerance, increasing) or \
                   IsSafeReport(report[1:], 0, tolerance-1, None)
        else:
            return IsSafeReport(report[1:], index, tolerance-1, increasing) or \
                   IsSafeReport(report[:1] + report[2:], index, tolerance-1, increasing)     

    else:
        if SafeLevel(current, next, increasing):
            return IsSafeReport(report, index+1, tolerance, increasing)
        else:
            #Launch algo with removing nodes around the problem and decreasing tolerance
            return IsSafeReport(report[:index] + report[index+1:], index -1, tolerance-1, increasing) or \
                   IsSafeReport(report[:index+1] + report[index+2:], index, tolerance-1, increasing)


def NumberOfSafeReport(reports, tolerance = 0):
    n_safe = 0
    for report in reports:
        if IsSafeReport(report, 0, tolerance, None):
            n_safe+=1
    return n_safe


# Question 1
print("Number of safe report : ", NumberOfSafeReport(reports))

# Question 2
print("Number of safe report with 1 tolerance: ", NumberOfSafeReport(reports, 1))






