from xml.dom.minidom import parse, parseString
import random, subprocess, platform, os, re, pdb

def get_data_from_file(filename):
    ''' read data from file into a list'''
    f = open(filename)
    filecontents = f.readlines()
    table = [line.replace('\n', '').replace('\r', '').replace(' ', '\t').split('\t') for line in filecontents]
    f.close()
    return table

class ProcessXML:

    def __init__(self, argos_xml = None):
        self.argos_xml = argos_xml
    
    def update_XML_parameters(self, parameters):
        xml = parse(self.argos_xml)
        
        if parameters.has_key("XMLBlock"):
            targetBlock =  xml.getElementsByTagName(parameters["XMLBlock"])
        else: 
            print "The target block is not specified in the input data file!!!!!!!!!!!!!!!!"
            return
            
        attrs = None
        count = 0
        #pdb.set_trace()
        for block in targetBlock:
            for idx in range(len(parameters.keys())):
                if  parameters.keys()[idx] != "XMLBlock":
                    if block.getAttribute(parameters.keys()[idx]) == '':
                        break
                    else:
                        attrs = block
                        count +=1
                        break
    
        if count > 1:
            print "More than one blocks have the same attribute!!!!!!!!!!!!!!!!!!!"            
            return
        if attrs == None:
            print "the target block does not exist !!!!!!!!!!!!!!!!"
            return
        for p in parameters:
            if p != "XMLBlock": 
                attrs.setAttribute(p,parameters[p])
        
        xml.writexml(open(self.argos_xml, 'w'))
        for p in parameters:
            if p != "XMLBlock":
                print p, "=", attrs.getAttribute(p), attrs.getAttribute(p) == parameters[p]

        print 'Updated setting parameters ...'

def processData(data):
    result={}
    #system = 'linux' if platform.system() == 'Linux' else 'mac'

    for line in data:
        if line[0] == 'Files':
            result[line[0]] = line[1:]
        else:
            result[line[0]]=line[1]
    return result

def update_Visulization(sourceFile, visualValue):
    
    #s = open(sourceFile)
    #filecontents = s.readlines()
    #for line in filecontents:
    #    print line
    pdb.set_trace()
        
    with open(sourceFile) as f1:
        if "<!-- <visualization>" in f1.read():
            if visualValue  == '1':
                f1.replace("<!-- <visualization>", "<visualization>")
                print "The visualization is updated to ON ..."
            elif visualValue  == '0':
                print "The default visualization has already been OFF *******"
            return
                
    with open(sourceFile) as f2:
        if "<!--<visualization>" in f2.read():
            if visualValue  == '1':
                f2.replace("<!--<visualization>", "<visualization>")
                print "The visualization is updated to ON ..."
            elif visualValue  == '0':
                print "The default visualization has already been OFF *******"
            return
    
    
    #targetFile = open(sourceFile, 'w')
    #targetFile.write(f)
    #targetFile.close()
    
def run(folder, dataFile):
    dataFileLocation = os.path.join(folder, dataFile)
    if not os.path.isfile(dataFileLocation):
        print "The file ", dataFileLocation, " does not exist!!!!!!!!!!!!!!!!!"
        
    parameterData = get_data_from_file(dataFileLocation)
    
    parameterDict = processData(parameterData)
    print 'parameterDict=', parameterDict
    
    if parameterDict.has_key('Files'):
        targetFiles = parameterDict['Files']
        del parameterDict['Files']
    
    for file in targetFiles:
        #pdb.set_trace()
        fileLocation = os.path.join(folder,file)+'.xml'
        if os.path.isfile(fileLocation):
            print "File: ", fileLocation
            if parameterDict.has_key("Visualization"):
                update_Visulization(fileLocation, parameterDict["Visualization"])
            else:  
                this_run = ProcessXML(fileLocation)
                this_run.update_XML_parameters(parameterDict)
        else:
            print fileLocation, ' does not exist!!!!!!!!!!!!!!!!!!!!'

    
if __name__ == "__main__":
    folder = raw_input('Please select a directory: ')
    #dataFile = raw_input('Please select the input file: ')
    #dataFiles = ['parameters_1024tags_setting.txt', 'parameters_16384tags_setting.txt']
    dataFiles = ['parameters_destination_noise.txt']
    for dataFile in dataFiles:
        run(folder, dataFile)
    






