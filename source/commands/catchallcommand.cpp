/*
 *  catchallcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 5/11/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "catchallcommand.h"

//**********************************************************************************************************************
vector<string> CatchAllCommand::setParameters(){	
	try {
		CommandParameter plabel("label", "String", "", "", "", "", "","",false,false); parameters.push_back(plabel);
		//can choose shared or sabund not both, so put them in the same chooseOnlyOneGroup
		CommandParameter pshared("shared", "InputTypes", "", "", "catchallInputs", "catchallInputs", "none","analysis-bestanalysis-models-bubble-summary",false,false,true); parameters.push_back(pshared);
		CommandParameter psabund("sabund", "InputTypes", "", "", "catchallInputs", "catchallInputs", "none","analysis-bestanalysis-models-bubble-summary",false,false,true); parameters.push_back(psabund);
        CommandParameter pgroups("groups", "String", "", "", "", "", "","",false,false); parameters.push_back(pgroups);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string CatchAllCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The catchall command interfaces mothur with the catchall program written by Linda Woodard, Sean Connolly and John Bunge.\n";
		helpString += "For more information about catchall refer to http://www.northeastern.edu/catchall/index.html \n";
		helpString += "The catchall executable must be in the same folder as your mothur executable. \n";
		helpString += "If you are a MAC or Linux user you must also have installed mono, a link to mono is on the webpage. \n";
		helpString += "The catchall command parameters are shared, sabund, groups and label.  shared or sabund is required. \n";
		helpString += "The label parameter is used to analyze specific labels in your input.\n";
		helpString += "The catchall command should be in the following format: \n";
		helpString += "catchall(sabund=yourSabundFile) \n";
		helpString += "Example: catchall(sabund=abrecovery.fn.sabund) \n";	
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string CatchAllCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "analysis") {  pattern = "[filename],_Analysis.csv"; } 
        else if (type == "bestanalysis") {  pattern = "[filename],_BestModelsAnalysis.csv"; }
        else if (type == "models") {  pattern = "[filename],_BestModelsAnalysis.csv"; }
        else if (type == "bubble") {  pattern = "[filename],_BubblePlot.csv"; }
        else if (type == "summary") {  pattern =  "[filename],catchall.summary"; }
        else if (type == "sabund") {  pattern =  "[filename],[distance],csv"; }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "CatchAllCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
CatchAllCommand::CatchAllCommand(){	
	try {
		abort = true; calledHelp = true;
		setParameters();
		//initialize outputTypes
		vector<string> tempOutNames;
		outputTypes["analysis"] = tempOutNames;
		outputTypes["bestanalysis"] = tempOutNames;
        outputTypes["models"] = tempOutNames;
		outputTypes["bubble"] = tempOutNames;
		outputTypes["summary"] = tempOutNames;
        outputTypes["sabund"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "CatchAllCommand");
		exit(1);
	}
}
/**************************************************************************************/
CatchAllCommand::CatchAllCommand(string option)  {	
	try {
		
		abort = false; calledHelp = false;   
		allLines = 1;
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
		
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["analysis"] = tempOutNames;
            outputTypes["bestanalysis"] = tempOutNames;
            outputTypes["models"] = tempOutNames;
            outputTypes["bubble"] = tempOutNames;
            outputTypes["summary"] = tempOutNames;
            outputTypes["sabund"] = tempOutNames;

			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("sabund");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["sabund"] = inputDir + it->second;		}
				}
				
				it = parameters.find("shared");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["shared"] = inputDir + it->second;		}
				}
			}

			//check for required parameters
			sabundfile = validParameter.validFile(parameters, "sabund");
			if (sabundfile == "not open") { sabundfile = ""; abort = true; }
			else if (sabundfile == "not found") { sabundfile = "";  }
			else { current->setSabundFile(sabundfile); }
			
			sharedfile = validParameter.validFile(parameters, "shared");
			if (sharedfile == "not open") { sharedfile = ""; abort = true; }
			else if (sharedfile == "not found") { sharedfile = "";   }
			else { current->setSharedFile(sharedfile); }
			
			string label = validParameter.valid(parameters, "label");
			if (label == "not found") { label = ""; }
			else { 
				if(label != "all") {  util.splitAtDash(label, labels);  allLines = 0;  }
				else { allLines = 1;  }
			}
		
			if ((sharedfile == "") && (sabundfile == "")) { 
				//is there are current file available for either of these?
				//give priority to shared, then sabund
				//if there is a current shared file, use it
				sharedfile = current->getSharedFile(); 
				if (sharedfile != "") {  m->mothurOut("Using " + sharedfile + " as input file for the shared parameter."); m->mothurOutEndLine(); }
				else { 
					sabundfile = current->getSabundFile(); 
					if (sabundfile != "") {  m->mothurOut("Using " + sabundfile + " as input file for the sabund parameter."); m->mothurOutEndLine(); }
					else { 
						m->mothurOut("No valid current files. You must provide a sabund or shared file before you can use the catchall command."); m->mothurOutEndLine(); 
						abort = true;
					}
				}
			}
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");
			if (outputDir == "not found"){	
				if (sabundfile != "") {  outputDir = util.hasPath(sabundfile); }
				else { outputDir = util.hasPath(sharedfile); }
			}
            
            string groups = validParameter.valid(parameters, "groups");
            if (groups == "not found") { groups = ""; }
            else { util.splitAtDash(groups, Groups);
                    if (Groups.size() != 0) { if (Groups[0]== "all") { Groups.clear(); } }  }

		}

	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "CatchAllCommand");
		exit(1);
	}
}
/**************************************************************************************/
int CatchAllCommand::execute() {	
	try {
		
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
		//get location of catchall
		path = current->getProgramPath();
		
        if (m->getDebug()) { m->mothurOut("[DEBUG]: mothur's path = " + path + "\n"); }
       
		savedOutputDir = outputDir;
		string catchAllCommandExe = "";
        string catchAllTest = "";
        string programName = "";
		#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
			if (outputDir == "") { outputDir = "./"; } //force full pathname to be created for catchall, this is necessary because if catchall is in the path it will look for input file whereever the exe is and not the cwd.
            catchAllTest = path + "CatchAllcmdL.exe";
		#else
			if (outputDir == "") { outputDir = ".\\"; } //force full pathname to be created for catchall, this is necessary because if catchall is in the path it will look for input file whereever the exe is and not the cwd.
            catchAllTest = path + "CatchAllcmdW.exe";
		#endif
		
        //test to make sure formatdb exists
		ifstream in;
		catchAllTest = util.getFullPathName(catchAllTest);
		bool ableToOpen = util.openInputFile(catchAllTest, in, "no error"); in.close();
		if(!ableToOpen) {	
            m->mothurOut(catchAllTest + " file does not exist. Checking path... \n");
            
            programName = "CatchAllcmdW.exe";
#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
            programName = "CatchAllcmdL.exe";
#endif
            Utils util;
            string cLocation = util.findProgramPath(programName);
            
            ifstream in2;
            ableToOpen = util.openInputFile(cLocation, in2, "no error"); in2.close();

            if(!ableToOpen) {
                programName = "catchall";
                
                string cLocation = util.findProgramPath(programName);
                
                ifstream in3;
                ableToOpen = util.openInputFile(cLocation, in3, "no error"); in3.close();
                
                if(!ableToOpen) { m->mothurOut("[ERROR]: " + cLocation + " file does not exist. mothur requires the catchall executable."); m->mothurOutEndLine();  return 0; }else {  m->mothurOut("Found catchall in your path, using " + cLocation + "\n"); catchAllTest = cLocation; }
            }
            else {  m->mothurOut("Found catchall in your path, using " + cLocation + "\n"); catchAllTest = cLocation; }
        }
        catchAllTest = util.getFullPathName(catchAllTest);
        
#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
        if (programName == "catchall") { catchAllCommandExe += "catchall "; }
        else {  catchAllCommandExe += "mono \"" + catchAllTest + "\" ";  }
#else
        catchAllCommandExe += "\"" + catchAllTest + "\" ";
#endif
        
		//prepare full output directory
		outputDir = util.getFullPathName(outputDir);
		
        if (m->getDebug()) { m->mothurOut("[DEBUG]: catchall location = " + catchAllCommandExe + "\n[DEBUG]: outputDir = " + outputDir + "\n"); }
        
		vector<string> inputFileNames;
		if (sharedfile != "") { inputFileNames = parseSharedFile(sharedfile);   }
		else {  inputFileNames.push_back(sabundfile);  }		
		
		for (int p = 0; p < inputFileNames.size(); p++) {
			if (inputFileNames.size() > 1) {
				m->mothurOutEndLine(); m->mothurOut("Processing group " + Groups[p]); m->mothurOutEndLine(); m->mothurOutEndLine();
			}
			
			InputData input(inputFileNames[p], "sabund", nullVector);
			SAbundVector* sabund = input.getSAbundVector();
			string lastLabel = sabund->getLabel();
							
			set<string> processedLabels;
			set<string> userLabels = labels;
			
            map<string, string> variables;
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(inputFileNames[p]));
			string summaryfilename = getOutputFileName("summary", variables);
			summaryfilename = util.getFullPathName(summaryfilename);
			
            if (m->getDebug()) { m->mothurOut("[DEBUG]: Input File = " + inputFileNames[p] + ".\n[DEBUG]: inputdata address = " + toString(&input) + ".\n[DEBUG]: sabund address = " + toString(&sabund) + ".\n"); } 
            
			ofstream out;
			util.openOutputFile(summaryfilename, out);	
			
			out << "label\tmodel\testimate\tlci\tuci" << endl;
            
            if (m->getDebug()) { string open = "no"; if (out.is_open()) { open = "yes"; } m->mothurOut("[DEBUG]: output stream is open = " + open + ".\n"); }
			
			//for each label the user selected
			while((sabund != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
				
						
				if(allLines == 1 || labels.count(sabund->getLabel()) == 1){
						m->mothurOut(sabund->getLabel());  m->mothurOutEndLine();
						
						//create catchall input file from mothur's inputfile
						string filename = process(sabund, inputFileNames[p]);
						string outputPath = util.getPathName(filename);
											
						//create system command
						string catchAllCommand = "";
						#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
							catchAllCommand += catchAllCommandExe + "\"" + filename + "\" \""  + outputPath + + "\" 1";
						#else
                            //removes extra '\\' catchall doesnt like that
                            vector<string> tempNames;
                            string tempFilename = filename;
                            util.splitAtDash(tempFilename, tempNames);
                            tempFilename = tempNames[0];
                            tempNames.clear();
                            string tempOutputPath = outputPath;
                            util.splitAtDash(tempOutputPath, tempNames);
                            tempOutputPath = tempNames[0];
                            if (tempOutputPath.length() > 0) { tempOutputPath = tempOutputPath.substr(0, tempOutputPath.length()-1); }
                            catchAllCommand += catchAllCommandExe + "\"" + tempFilename + "\" \""  + tempOutputPath + "\" 1";
                            catchAllCommand = "\"" + catchAllCommand + "\"";
						#endif
                        
                        if (m->getDebug()) {  m->mothurOut("[DEBUG]: catchall command = " + catchAllCommand + ". About to call system.\n"); }
                    
						//run catchall
						system(catchAllCommand.c_str());
                    
                        if (m->getDebug()) { m->mothurOut("[DEBUG]: back from system call. Keeping file: " + filename + ".\n"); }
                    
                        if (!m->getDebug()) { util.mothurRemove(filename); }

					
						filename = util.getRootName(filename); filename = filename.substr(0, filename.length()-1); //rip off extra .
						if (savedOutputDir == "") { filename = util.getSimpleName(filename); }
					
                        variables["[filename]"] = filename;
						outputNames.push_back(getOutputFileName("analysis", variables)); outputTypes["analysis"].push_back(getOutputFileName("analysis", variables));
						outputNames.push_back(getOutputFileName("bestanalysis", variables)); outputTypes["bestanalysis"].push_back(getOutputFileName("bestanalysis", variables));
                        outputNames.push_back(getOutputFileName("models", variables)); outputTypes["models"].push_back(getOutputFileName("models", variables));
                        outputNames.push_back(getOutputFileName("bubble", variables)); outputTypes["bubble"].push_back(getOutputFileName("bubble", variables));
                        
                        if (m->getDebug()) { m->mothurOut("[DEBUG]: About to create summary file for: " + filename + ".\n[DEBUG]: sabund label = " + sabund->getLabel() + ".\n"); }
                    
						createSummaryFile(filename + "_BestModelsAnalysis.csv", sabund->getLabel(), out);
                    
                        if (m->getDebug()) { m->mothurOut("[DEBUG]: Done creating summary file.\n"); }
											
						if (m->getControl_pressed()) { out.close(); for (int i = 0; i < outputNames.size(); i++) {util.mothurRemove(outputNames[i]);	}  delete sabund;  return 0; }

						processedLabels.insert(sabund->getLabel());
						userLabels.erase(sabund->getLabel());
				}
				
				if ((util.anyLabelsToProcess(sabund->getLabel(), userLabels, "") ) && (processedLabels.count(lastLabel) != 1)) {
						string saveLabel = sabund->getLabel();
						
						delete sabund;		
						sabund = (input.getSAbundVector(lastLabel));
						
						m->mothurOut(sabund->getLabel());  m->mothurOutEndLine();
						

						//create catchall input file from mothur's inputfile
						string filename = process(sabund, inputFileNames[p]);
						string outputPath = util.getPathName(filename);
											
						//create system command
						string catchAllCommand = "";
						#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
                            catchAllCommand += catchAllCommandExe + "\"" + filename + "\" \""  + outputPath + + "\" 1";
						#else
                            //removes extra '\\' catchall doesnt like that
                            vector<string> tempNames;
                            string tempFilename = filename;
                            util.splitAtDash(tempFilename, tempNames);
                            tempFilename = tempNames[0];
                            tempNames.clear();
                            string tempOutputPath = outputPath;
                            util.splitAtDash(tempOutputPath, tempNames);
                            tempOutputPath = tempNames[0];
							if (tempOutputPath.length() > 0) { tempOutputPath = tempOutputPath.substr(0, tempOutputPath.length()-1); }
							catchAllCommand += catchAllCommandExe + "\"" + tempFilename + "\" \""  + tempOutputPath + "\" 1";
							catchAllCommand = "\"" + catchAllCommand + "\"";
						#endif
                        
                        if (m->getDebug()) {  m->mothurOut("[DEBUG]: catchall command = " + catchAllCommand + ". About to call system.\n"); }
                    
						//run catchall
						system(catchAllCommand.c_str());
                    
                        if (m->getDebug()) { m->mothurOut("[DEBUG]: back from system call. Keeping file: " + filename + ".\n"); }
                    
                        if (!m->getDebug()) { util.mothurRemove(filename); }

					
						filename = util.getRootName(filename); filename = filename.substr(0, filename.length()-1); //rip off extra .
						if (savedOutputDir == "") { filename = util.getSimpleName(filename); }
					
                        variables["[filename]"] = filename;
                        outputNames.push_back(getOutputFileName("analysis", variables)); outputTypes["analysis"].push_back(getOutputFileName("analysis", variables));
                        outputNames.push_back(getOutputFileName("bestanalysis", variables)); outputTypes["bestanalysis"].push_back(getOutputFileName("bestanalysis", variables));
                        outputNames.push_back(getOutputFileName("models", variables)); outputTypes["models"].push_back(getOutputFileName("models", variables));
                        outputNames.push_back(getOutputFileName("bubble", variables)); outputTypes["bubble"].push_back(getOutputFileName("bubble", variables));
                        
                    
                        if (m->getDebug()) { m->mothurOut("[DEBUG]: About to create summary file for: " + filename + ".\n[DEBUG]: sabund label = " + sabund->getLabel() + ".\n"); }
                    
						createSummaryFile(filename + "_BestModelsAnalysis.csv", sabund->getLabel(), out);
                    
                        if (m->getDebug()) { m->mothurOut("[DEBUG]: Done creating summary file.\n"); }
					
						if (m->getControl_pressed()) { out.close(); for (int i = 0; i < outputNames.size(); i++) {util.mothurRemove(outputNames[i]);	}   delete sabund;  return 0; }

						processedLabels.insert(sabund->getLabel());
						userLabels.erase(sabund->getLabel());
						
						//restore real lastlabel to save below
						sabund->setLabel(saveLabel);
				}
				
				
				lastLabel = sabund->getLabel();	
				
				delete sabund;		
				sabund = (input.getSAbundVector());
			}
			
			//output error messages about any remaining user labels
			set<string>::iterator it;
			bool needToRun = false;
			for (it = userLabels.begin(); it != userLabels.end(); it++) {  
				m->mothurOut("Your file does not include the label " + *it); 
				if (processedLabels.count(lastLabel) != 1) {
					m->mothurOut(". I will use " + lastLabel + ".");  m->mothurOutEndLine();
					needToRun = true;
				}else {
					m->mothurOut(". Please refer to " + lastLabel + ".");  m->mothurOutEndLine();
				}
			}
			
			//run last label if you need to
			if (needToRun )  {
				if (sabund != NULL) {	delete sabund;	}
				sabund = (input.getSAbundVector(lastLabel));
				
				m->mothurOut(sabund->getLabel());  m->mothurOutEndLine();
				
				//create catchall input file from mothur's inputfile
				string filename = process(sabund, inputFileNames[p]);
				string outputPath = util.getPathName(filename);
				
				//create system command
				string catchAllCommand = "";
				#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
                    catchAllCommand += catchAllCommandExe + "\"" + filename + "\" \""  + outputPath + + "\" 1";
				#else
                    //removes extra '\\' catchall doesnt like that
                    vector<string> tempNames;
                    string tempFilename = filename;
                    util.splitAtDash(tempFilename, tempNames);
                    tempFilename = tempNames[0];
                    tempNames.clear();
                    string tempOutputPath = outputPath;
                    util.splitAtDash(tempOutputPath, tempNames);
                    tempOutputPath = tempNames[0];
                    if (tempOutputPath.length() > 0) { tempOutputPath = tempOutputPath.substr(0, tempOutputPath.length()-1); }
                    catchAllCommand += catchAllCommandExe + "\"" + tempFilename + "\" \""  + tempOutputPath + "\" 1";
                    catchAllCommand = "\"" + catchAllCommand + "\"";
				#endif
                
                if (m->getDebug()) {  m->mothurOut("[DEBUG]: catchall command = " + catchAllCommand + ". About to call system.\n"); }
				
				//run catchall
				system(catchAllCommand.c_str());
                
                if (m->getDebug()) { m->mothurOut("[DEBUG]: back from system call. Keeping file: " + filename + ".\n"); }
				
				if (!m->getDebug()) { util.mothurRemove(filename); }
				
				filename = util.getRootName(filename); filename = filename.substr(0, filename.length()-1); //rip off extra .
				if (savedOutputDir == "") { filename = util.getSimpleName(filename); }
				
				variables["[filename]"] = filename;
                outputNames.push_back(getOutputFileName("analysis", variables)); outputTypes["analysis"].push_back(getOutputFileName("analysis", variables));
                outputNames.push_back(getOutputFileName("bestanalysis", variables)); outputTypes["bestanalysis"].push_back(getOutputFileName("bestanalysis", variables));
                outputNames.push_back(getOutputFileName("models", variables)); outputTypes["models"].push_back(getOutputFileName("models", variables));
                outputNames.push_back(getOutputFileName("bubble", variables)); outputTypes["bubble"].push_back(getOutputFileName("bubble", variables));
                if (m->getDebug()) { m->mothurOut("[DEBUG]: About to create summary file for: " + filename + ".\n[DEBUG]: sabund label = " + sabund->getLabel() + ".\n"); }
                
				createSummaryFile(filename + "_BestModelsAnalysis.csv", sabund->getLabel(), out);
                
                if (m->getDebug()) { m->mothurOut("[DEBUG]: Done creating summary file.\n"); }
				
				delete sabund;
			}
			
			out.close();
			
			if (m->getControl_pressed()) { for (int i = 0; i < outputNames.size(); i++) {util.mothurRemove(outputNames[i]);	} return 0; }
				
		}
		
		if (sharedfile == "") {  
			string summaryfilename = savedOutputDir + util.getRootName(util.getSimpleName(inputFileNames[0])) + "catchall.summary";
			summaryfilename = util.getFullPathName(summaryfilename);
			outputNames.push_back(summaryfilename); outputTypes["summary"].push_back(summaryfilename);
		}else { //combine summaries
			vector<string> sumNames;
			for (int i = 0; i < inputFileNames.size(); i++) {
				sumNames.push_back(util.getFullPathName(outputDir + util.getRootName(util.getSimpleName(inputFileNames[i])) + "catchall.summary"));
			}
			string summaryfilename = combineSummmary(sumNames);
			outputNames.push_back(summaryfilename); outputTypes["summary"].push_back(summaryfilename);
		}
		
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}	
		m->mothurOutEndLine();
		

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
string CatchAllCommand::process(SAbundVector* sabund, string file1) {
	try {
        map<string, string> variables;
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(file1));
        variables["[distance]"] = sabund->getLabel();
        string filename = getOutputFileName("sabund", variables);
		filename = util.getFullPathName(filename);
	
		ofstream out;
		util.openOutputFile(filename, out);
		
        if (m->getDebug()) { m->mothurOut("[DEBUG]: Creating " + filename + " file for catchall, shown below.\n\n"); }
        
		for (int i = 1; i <= sabund->getMaxRank(); i++) {
			int temp = sabund->get(i);
			
			if (temp != 0) {
				out << i << "," << temp << endl;
                if (m->getDebug()) { m->mothurOut(toString(i) + "," + toString(temp) + "\n"); }
			}
		}
		out.close();
        
        if (m->getDebug()) { m->mothurOut("[DEBUG]: Done creating " + filename + " file for catchall, shown above.\n\n"); }
		
		return filename;
	
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "process");
		exit(1);
	}
}
//*********************************************************************************************************************
string CatchAllCommand::combineSummmary(vector<string>& outputNames) {
	try {
		
		ofstream out;
        map<string, string> variables;
        variables["[filename]"] = savedOutputDir + util.getRootName(util.getSimpleName(sharedfile));
        string combineFileName = getOutputFileName("summary", variables);
		
		//open combined file
		util.openOutputFile(combineFileName, out);
		
		out << "label\tgroup\tmodel\testimate\tlci\tuci" << endl;
		
		//open each groups summary file
		string newLabel = "";
		int numLines = 0;
		map<string, vector<string> > files;
		for (int i=0; i<outputNames.size(); i++) {
			vector<string> thisFilesLines;
			
			ifstream temp;
			util.openInputFile(outputNames[i], temp);
			
			//read through first line - labels
			util.getline(temp);			
			util.gobble(temp);
			
			//for each label
			while (!temp.eof()) {
				
				string thisLine = "";
				string tempLabel;
				
				for (int j = 0; j < 5; j++) {  
					temp >> tempLabel; 
					
					//save for later
					if (j == 1) { thisLine += Groups[i] + "\t" + tempLabel + "\t";	}
					else{  thisLine += tempLabel + "\t";	}
				}
				
				thisLine += "\n";
				
				thisFilesLines.push_back(thisLine);
				
				util.gobble(temp);
			}
			
			files[outputNames[i]] = thisFilesLines;
			
			numLines = thisFilesLines.size();
			
			temp.close();
			util.mothurRemove(outputNames[i]);
		}
		
		//for each label
		for (int k = 0; k < numLines; k++) {
			
			//grab summary data for each group
			for (int i=0; i<outputNames.size(); i++) {
				out << files[outputNames[i]][k];
			}
		}	
		
		
		out.close();
		
		//return combine file name
		return combineFileName;
		
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "combineSummmary");
		exit(1);
	}
}
//**********************************************************************************************************************
int CatchAllCommand::createSummaryFile(string file1, string label, ofstream& out) {
	try {
		
		ifstream in;
		bool able = util.openInputFile(file1, in, "noerror");
		
		if (!able) {  m->mothurOut("[ERROR]: the catchall program did not run properly. Please check to make sure it is located in the same folder as your mothur executable.");m->mothurOutEndLine();  m->setControl_pressed(true);  return 0; }
			
		if (!in.eof()) {
			
			string header = util.getline(in); util.gobble(in);
			
			int pos = header.find("Total Number of Observed Species =");
			string numString = "";
			
			
			if (pos == string::npos) { m->mothurOut("[ERROR]: cannot parse " + file1); m->mothurOutEndLine(); }
			else {
				//pos will be the position of the T in total, so we want to count to the position of =
				pos += 34;
				char c=header[pos];
				while (c != ','){
					if (c != ' ') {
						numString += c;
					}
					pos++;
					c=header[pos];
					
					//sanity check
					if (pos > header.length()) { m->mothurOut("Cannot find number of OTUs in " + file1); m->mothurOutEndLine(); in.close(); return 0; }
				}
			}
															  
			string firstline = util.getline(in); util.gobble(in);
			vector<string> values;
			util.splitAtComma(firstline, values);
			
			values.pop_back(); //last value is always a blank string since the last character in the line is always a ','
			
			if (values.size() == 1) { //grab next line if firstline didn't have what you wanted
				string secondline = util.getline(in); util.gobble(in);
				values.clear();
				util.splitAtComma(secondline, values);
				
				values.pop_back(); //last value is always a blank string since the last character in the line is always a ','
			}
			
			if (values.size() == 1) { //still not what we wanted fill values with numOTUs
				values.resize(8, "");
				values[1] = "Sobs";
				values[4] = numString;
				values[6] = numString;
				values[7] = numString;
			}
			
			if (values.size() < 8) { values.resize(8, ""); }
			
			out << label << '\t' << values[1] << '\t' << values[4] << '\t' << values[6] << '\t' << values[7] << endl;
		}
		
		in.close();
		
		return 0;
		
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "createSummaryFile");
		exit(1);
	}
}
//**********************************************************************************************************************
vector<string> CatchAllCommand::parseSharedFile(string filename) {
	try {
		vector<string> filenames;
		
		//read first line
		InputData input(filename, "sharedfile", Groups);
		SharedRAbundVectors* shared = input.getSharedRAbundVectors();
        Groups = shared->getNamesGroups();
		string sharedFileRoot = outputDir + util.getRootName(util.getSimpleName(filename));
		
		//clears file before we start to write to it below
		for (int i=0; i<Groups.size(); i++) {
			util.mothurRemove((sharedFileRoot + Groups[i] + ".sabund"));
			filenames.push_back((sharedFileRoot + Groups[i] + ".sabund"));
		}
    
		while(shared != NULL) {
            vector<SharedRAbundVector*> lookup = shared->getSharedRAbundVectors();
			for (int i = 0; i < lookup.size(); i++) {
				SAbundVector sav = lookup[i]->getSAbundVector();
				ofstream out;
				util.openOutputFileAppend(sharedFileRoot + Groups[i] + ".sabund", out);
				sav.print(out);
				out.close();
			}
			
            for (int i = 0; i < lookup.size(); i++) {  if (lookup[i] != NULL) { delete lookup[i]; } lookup[i] = NULL; }
			shared = input.getSharedRAbundVectors();
		}
		
		return filenames;
	}
	catch(exception& e) {
		m->errorOut(e, "CatchAllCommand", "parseSharedFile");
		exit(1);
	}
}
/**************************************************************************************/
