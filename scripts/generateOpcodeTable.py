import requests
from bs4 import BeautifulSoup
import csv

'''
    Script to parse the instruction set table to create the instruction lookup table for the CPU class.
    Data is scraped from https://www.masswerk.at/6502/6502_instruction_set.html

'''

def printList(myList):
    for i in myList:
        print(i)

def createCSV(instructions, finalName):
    print("Creating CSV " + finalName + ".csv")
    data = []
    headers = [' ', '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F']
    tmp = headers
    for i in range(len(instructions)):
        if (i%16 == 0):
            data.append(tmp)
            tmp = []
            tmp.append('%s' % headers[i//16 + 1])

        string = ""
        for col in range(len(instructions[i])):
            string += str(instructions[i][col]) + " "
        #tmp.append("%s %s" % (instructions[i][0], instructions[i][1]))
        tmp.append(string)

        #to take care of the last row and add it to the CSV
        if (i == len(instructions) - 1):
            data.append(tmp)

    with open(finalName + ".csv", 'w', newline='') as file:
        writer = csv.writer(file, delimiter=',')
        writer.writerows(data)


def generateInstructionList(myList):

    instructions = list()

    
    addressMode = ""
    instr = "" # the function in the cpu class
    addressModeFunc = "" #the function in the cpu class

    for i in myList:
        instr = i.split()[0]
        instr = instr.upper().replace("\n","")
        addressMode = i.split()[1]
        
        addressModeFunc = ""
        if addressMode == "zpg":
            addressModeFunc = "ZPG"
        elif addressMode == "impl":
            addressModeFunc = "IMPL"
        elif addressMode == "abs,X":
            addressModeFunc = "ABSX"
        elif addressMode == "A":
            addressModeFunc = "IMPL"
        elif addressMode == "abs,Y":
            addressModeFunc = "ABSY"
        elif addressMode == "ind":
            addressModeFunc = "IND"
        elif addressMode == "rel":
            addressModeFunc = "REL"
        elif addressMode == "abs":
            addressModeFunc = "ABS"
        elif addressMode == "zpg,X":
            addressModeFunc = "ZPGX"
        elif addressMode == "#":
            addressModeFunc = "IMM"
        elif addressMode == "X,ind":
            addressModeFunc = "INDX"
        elif addressMode == "zpg,Y":
            addressModeFunc = "ZPGY"
        elif addressMode == "ind,Y":
            addressModeFunc = "INDY"
        else:
            print("MISSED: " + addressMode)

        assert(addressModeFunc != "")

        instructions.append((instr,addressModeFunc))
    
    return instructions

def parseInstructionSetPage():
    url = "https://www.masswerk.at/6502/6502_instruction_set.html"

    page = requests.get(url)
    soup = BeautifulSoup(page.content,features="html.parser")
    table = soup.find(id = 'opctable')
    table_body = table.find('tbody')

    instr = []
    for row in table_body.find_all('tr'):
        for col in row.find_all('td'):
            op = str(col)
            if "hi-nibble" in op:
                continue
            
            instruction = ""
            if "undef" in op:
                #unofficial instruction
                instruction = "NOP impl"

            else:
                instruction = op[4:-5]

            instr.append(instruction)

    return instr


def getCycleInfo(instructions):
    text = open("operationCycleInfo.txt", "r")
    lines = text.readlines()
    text.close()

    mapping = {
        'accumulator': 'IMPL',
        'immidiate' : 'IMM',
        'implied'   : 'IMPL',
        'zeropage'  : 'ZPG',
        'zeropage,X': 'ZPGX',
        'zeropage,Y': 'ZPGY',
        'absolute'  : 'ABS',
        'absolute,X': 'ABSX',
        'absolute,Y': 'ABSY',
        'indirect'  : 'IND',
        '(indirect,X)': 'INDX',
        '(indirect),Y': 'INDY',
        'relative'  : 'REL'
    }

    def find_all(lines, instr):
        data = []
        for i in lines:
            if instr in i:
                data.append(i)
        return data


    data = []
    instrNum = 0
    finalInstructions = []
    for (instr, addressMode) in instructions:
        #print(instr)

        #get cycle info from the website text
        data = find_all(lines,instr)

        #data[0] = instruction title
        #data [1..len(data)-1] = addressing mode info 

        cycleInfo = data[1:]
        numCycles = 0
        needsExtraCycle = False

        foundMode = False
        
        for mode in cycleInfo:
            info = mode.split()
            
                
            if (addressMode == mapping[info[0]]):
                starCount = info[-1].count('*')
                if  starCount == 2:
                    pass

                elif starCount == 1:
                    needsExtraCycle = True
                

                numCycles = info[-1][0]
                finalInstructions.append("%s %s %s %s" %(instr, addressMode, numCycles, str(needsExtraCycle).lower()))
                foundMode = True
                break

        if not foundMode:
            print("OOPS: missed " + instr + " " + addressMode + " [0x" + str(hex((instrNum >> 4) & 0xf)) + str(hex(instrNum & 0xf)) + "]")
            exit(-1)
       
        instrNum += 1

    #entry format: (name/operation, addrmode, numCycles, extraPage?)
    return finalInstructions

def generateCPPArray(finalList):
    '''
        Generate the final class definition of the opcode table
    '''

    f = open("opcode_lookup.txt", "w")

  
    counter = 0
    for string in finalList:
        string = string.split()

        instr = string[0]
        addrmode = string[1]
        numCycles = string[2]
        extraPage = string[3]
        struct = "{" + "\"" + instr + "\", " + "&cpu::" + instr + ", " + "&cpu::" + addrmode + ", " + numCycles + ", " + extraPage + "}, " 
        f.write(struct)


        counter+=1

        if (counter % 16 == 0):
            f.write("\n")


    f.close()


if __name__ == "__main__":
    instr = parseInstructionSetPage()
    instrList = generateInstructionList(instr)
    
    #createCSV(instrList, "Instructions")

    finalInstructions = getCycleInfo(instrList)
    
    #createCSV(finalInstructions, "final")

    generateCPPArray(finalInstructions)