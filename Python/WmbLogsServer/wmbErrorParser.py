import re
import StringIO

L = """( ['MQROOT' : 0x7f0260190fe0] (0x01000000:Name):RecoverableException = ( (0x03000000:NameValue):File = '/build/slot1/S800_P/src/DataFlowEngine/TemplateNodes/ImbRequestTemplateNode.cpp' (CHARACTER) (0x03000000:NameValue):Line = 532 (INTEGER) (0x03000000:NameValue):Function = 'ImbRequestTemplateNode::processMessageAssemblyToFailure' (CHARACTER) (0x03000000:NameValue):Type = 'ComIbmFileReadNode' (CHARACTER) (0x03000000:NameValue):Name = 'ReceiveFax#FCMComposite_1_4' (CHARACTER) (0x03000000:NameValue):Label = 'ReceiveFax.File Read' (CHARACTER) (0x03000000:NameValue):Catalog = 'BIPmsgs' (CHARACTER) (0x03000000:NameValue):Severity = 3 (INTEGER) (0x03000000:NameValue):Number = 2230 (INTEGER) (0x03000000:NameValue):Text = 'Caught exception and rethrowing' (CHARACTER) (0x01000000:Name ):RecoverableException = ( (0x03000000:NameValue):File = 'FileReadContext.java' (CHARACTER) (0x03000000:NameValue):Line = 264 (INTEGER) (0x03000000:NameValue):Function = 'checkDirectories' (CHARACTER) (0x03000000:NameValue):Type = '' (CHARACTER) (0x03000000:NameValue):Name = '' (CHARACTER) (0x03000000:NameValue):Label = '' (CHARACTER) (0x03000000:NameValue):Catalog = 'BIPmsgs' (CHARACTER) (0x03000000:NameValue):Severity = 1 (INTEGER) (0x03000000:NameValue):Number = 4988 (INTEGER) (0x03000000:NameValue):Text = 'directory invalid: null' (CHARACTER) (0x01000000:Name ):Insert = ( (0x03000000:NameValue):Type = 5 (INTEGER) (0x03000000:NameValue):Text = '' (CHARACTER) ) ) ) )#015 Environment of the flow is : ( ['MQROOT' : 0x7f026019aa30] (0x01000000:Name):Variable = ( (0x01000000:Name):FileData = ) ) """
L2 = """( ['MQROOT' : 0x7f971c180ab0]   (0x01000000:Name):RecoverableException = (     (0x03000000:NameValue):File                 = '/build/slot1/S800_P/src/DataFlowEngine/ImbComputeNode.cpp' (CHARACTER)     (0x03000000:NameValue):Line                 = 497 (INTEGER)     (0x03000000:NameValue):Function             = 'ImbComputeNode::evaluate' (CHARACTER)     (0x03000000:NameValue):Type                 = 'ComIbmComputeNode' (CHARACTER)     (0x03000000:NameValue):Name                 = 'FAXECMServiceSFDCMainFlow#FCMComposite_1_8.CreateFormImageSFDCSubFlow#FCMComposite_1_3' (CHARACTER)     (0x03000000:NameValue):Label                = 'FAXECMServiceSFDCMainFlow.FormSvcToSFDC' (CHARACTER)     (0x03000000:NameValue):Catalog              = 'BIPmsgs' (CHARACTER)     (0x03000000:NameValue):Severity             = 3 (INTEGER)     (0x03000000:NameValue):Number               = 2230 (INTEGER)     (0x03000000:NameValue):Text                 = 'Caught exception and rethrowing' (CHARACTER)     (0x01000000:Name     ):RecoverableException = (       (0x03000000:NameValue):File          = '/build/slot1/S800_P/src/DataFlowEngine/ImbRdl/ImbRdlStatementGroup.cpp' (CHARACTER)       (0x03000000:NameValue):Line          = 666 (INTEGER)       (0x03000000:NameValue):Function      = 'SqlStatementGroup::execute' (CHARACTER)       (0x03000000:NameValue):Type          = '' (CHARACTER)       (0x03000000:NameValue):Name          = '' (CHARACTER)       (0x03000000:NameValue):Label         = '' (CHARACTER)       (0x03000000:NameValue):Catalog       = 'BIPmsgs' (CHARACTER)       (0x03000000:NameValue):Severity      = 3 (INTEGER)       (0x03000000:NameValue):Number        = 2488 (INTEGER)       (0x03000000:NameValue):Text          = 'Error detected, rethrowing' (CHARACTER)       (0x01000000:Name     ):Insert        = (         (0x03000000:NameValue):Type = 5 (INTEGER)         (0x03000000:NameValue):Text = '.CreateFormImageSFDCSubFlow_"""

MSG_LINE_NV = r"\s*\(0x\w+:NameValue\):(?P<key>[^=]+)=(?P<value>.*)\([A-Z]+\)$"
MSG_LINE_SUB = r"\s*\(0x\w+:Name\s*\):(?P<key>[^=]+)=\s*\($"

reMsgNV = re.compile(MSG_LINE_NV)
reMsgSub = re.compile(MSG_LINE_SUB)

def parseFormatWmbError (errorLine):
    result = StringIO.StringIO()

    #message content extracted, preformat lines
    msg = errorLine.replace("( ", "(\n").replace(") ", ")\n")

    lineLevel = 0
    for line in msg.splitlines():
        #print(">" + line)
        match = reMsgNV.match(line)
        if match != None:
            #NameValue line: extract info and return
            result.write ("".ljust(lineLevel, "\t") + "%s = %s" % (match.group("key").strip(), match.group("value").strip()))
            result.write("\n")
        else:
            match = reMsgSub.match(line)
            if match != None:
                #SUBGROUP: ident++
                result.write ("".ljust(lineLevel, "\t") + "%s :" % match.groups())
                result.write("\n")
                lineLevel += 1
            elif ")" == line.strip():
                #CLOSING PAREN: end of block, ident--
                lineLevel -= 1
                if lineLevel < 0: lineLevel = 0
            else:
                #UNMATCHED: print as is
                #result.write("[?]")
                result.write("".ljust(lineLevel, "\t") + line)
                result.write("\n")
     
    r = result.getvalue()
    result.close()
    return r

###########################################
##  Main()
###########################################

if __name__ == '__main__':
    print (parseFormatWmbError (L2))
