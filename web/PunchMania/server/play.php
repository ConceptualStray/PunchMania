<?php



//dummy serial connection for now;
class serial{
	public function sendMessage($msg){
		echo $msg;
	}
}

use master\cache\cache;
use master\methodResolver;

$serial=new serial();
$c=new cache();

$r=new methodResolver(['id','offset','lenght']);

//load notes from file with the respect of the offset and lenght
//offset and lenght are in miliseconds
$songOffset=$r->offset;
$songEnd=$r->lenght+$songOffset;

$nData=parse_ini_file('notes.ini');

$notes=[];
 
//millis
$startLeadTime=1*1000;
$nCounter=0;
$absoluteTime=0;
$timeSinceStart=0;
$uploadTimes=[];
//get only notes in the range of user selection
foreach($nData as $timestamp){
	$absoluteTime+=$timestamp;
	if($absoluteTime>=$songOffset && $absoluteTime<=$songEnd and $timeSinceStart>=$startLeadTime){
		$timeSinceStart+=$timestamp;
		$notes[]=$timestamp;
		$nCounter++;
		if($nCounter==50){
			$uploadTimes[]=$timeSinceStart;
			$nCounter=0;
		}
	}
}

//add deviceOpen somewhere here

//send notes to the client
//max 100 per batch if there's more, then wait untill time has passed and load more


//send first 100 notes
foreach($notes as $i=>$note){
	if($i==100){
		$serial->sendMessage('s');
		$c->set('key','s');
		$c->set('time',time());
		$sTime=time();
		break;
	}
	$serial->sendMessage($note."\n");
	unset($notes[$i]);
}

$tindex=0;
while(count($notes)>0){
	$diff=time()-$sTime;
	if($diff>=$uploadTimes[$tindex]){
		sendNotes($notes,$serial);
		unset($uploadTimes[$tindex]);
		$tindex++;
	}
	sleep(1);
}

function sendNotes(&$notes,&$serial):void{
	$c=0;
	foreach($notes as $i=>$note){
		if($c==50){
			return;
		}
		$serial->sendMessage($note."\n");
		unset($notes[$i]);
	}
}

