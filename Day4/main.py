from enum import Enum

matrice = []

with open("input.txt", 'r') as file:
    for line in file:
        matrice += [line.strip()]

width = len(matrice[0]) #140
length = len(matrice)   #140

#################### Question1 ###############################
directions = {
    "TOP": (-1,0),
    "TOP_RIGHT": (-1,1),
    "RIGHT": (0,1),
    "BOT_RIGHT": (1,1),
    "BOT": (1,0),
    "BOT_LEFT": (1,-1),
    "LEFT": (0,-1),
    "TOP_LEFT": (-1,-1),
}

#global varibale
count = 0

def CountXMAS(i, j, str, current_direction):
    global count

    if i>=length or i <0:
        return
    if j>=length or j <0:
        return

    letter = matrice[i][j]

    if str == "" and  letter == 'X':
        str = "X"
        for direction, value in directions.items():
            next_i = i+value[0]
            next_j = j+value[1]
            CountXMAS(next_i, next_j, str, direction)
    elif str == "X" and letter == 'M':
        str = "XM"
        next_i = i+directions[current_direction][0]
        next_j = j+directions[current_direction][1]
        CountXMAS(next_i, next_j, str, current_direction)
    elif str == "XM" and letter == 'A':
        str = "XMA"
        next_i = i+directions[current_direction][0]
        next_j = j+directions[current_direction][1]
        CountXMAS(next_i, next_j, str, current_direction)
    elif str == "XMA" and letter == 'S':
        count +=1
        return
    else:
        return

for i in range(length):
    for j in range(width):
        CountXMAS(i,j,"", None)
        

print("Number of XMAS: ", count)

#################### Question2 ###############################
count_X = 0

CyclicPosition = [ (-1,1), (1,1), (1,-1), (-1,-1)] # [TOP_RIGHT, BOT_RIGHT, BOT_LEFT, TOP_LEFT]

def CountShapeXMAS(i, j):
    global count_X

    if matrice[i][j] == 'A':
        for k in range(4):
            if matrice[i+CyclicPosition[k%4][0]][j+CyclicPosition[k%4][1]] == 'M' and \
               matrice[i+CyclicPosition[(k+1)%4][0]][j+CyclicPosition[(k+1)%4][1]] == 'M' and \
               matrice[i+CyclicPosition[(k+2)%4][0]][j+CyclicPosition[(k+2)%4][1]] == 'S' and \
               matrice[i+CyclicPosition[(k+3)%4][0]][j+CyclicPosition[(k+3)%4][1]] == 'S' :
               count_X +=1



for i in range(1,length-1):
    for j in range(1,width-1):
        CountShapeXMAS(i,j)

print("Number of MAS in X shape: ", count_X)

        