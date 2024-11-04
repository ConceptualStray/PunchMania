#include <Arduino.h>

bool stringToBool(String str) {
    str.toLowerCase();  // Make it case-insensitive
    return (str == "true" || str == "1");
}

//c.command
String commandConfig(String command){
	if(command.startsWith("offset")){
		//get offset from cmd
		String data=command.substring(9);
		offset=data.toFloat()*1000;
		return String (offset);
	}

	return "No command";
}

// m.command
String commandMusic(String command){
	if(command.equals("start")){
		zeroTime=millis()+startOffset;
		isPlaying=true;
		return "Start";
	}

	if(command.equals("stop")){
		isPlaying=false;
		return "Stopped";
	}
	
	return "No command";
}

//d.command
const int numGroups = 6;
const int ledsPerGroup = 8;
String commandDebug(String command){
	int ledNumber = command.substring(1).toInt();
	toggleLed(ledNumber);
	return String(ledNumber);
}

//n.command
String commandNotes(String command){
	if(command.startsWith("add")){
		String data=command.substring(3);
		//strip brackets
		int bStart=data.indexOf("[");
		int bEnd=data.indexOf("]");
		if(bStart!=-1 && bEnd!=-1){
			data.remove(bStart,1);
			data.remove(bEnd - bStart - 1, 1);
		}

		notes[totalNotesCnt][0]=data.substring(0,data.indexOf(",")).toFloat();
		notes[totalNotesCnt][1]=data.substring(data.indexOf(",")+1).toInt();

		totalNotesCnt=(totalNotesCnt+1)%maxNotesInMem;
		return String(totalNotesCnt);
	}

	if(command.equals("clear")){
		totalNotesCnt=0;
		for(int i=0;i<maxNotesInMem;i++){
			notes[i][0]=-1;
			notes[i][1]=-1;
		}
		return "Cleared";
	}

	return "No command";
}

void commandParse(String pointer,String command){
	String response="No command";
	if(pointer.equals("c")){
		response=commandConfig(command);
	}else if(pointer.equals("m")){
		response=commandMusic(command);
	}else if(pointer.equals("d")){
		response=commandDebug(command);
	}else if(pointer.equals("n")){
		response=commandNotes(command);
	}
	Serial.println(response);
	//clear the buffer	
	while (Serial.available() > 0) {
		Serial.read();
	}
}
