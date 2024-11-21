<?php
$id=$_POST['id'];

$offset=$_POST['offset'];
$allowDoubles=$_POST['allowDoubles'];
$timeSongStart=$_POST['startTime']*1000;
$timeSongEnd=$_POST['endTime']*1000;


$configFile='data/'.$id.'/config.ini';
$config=parse_ini_file($configFile);
$config['offset']=$offset;
$config['allowDoubles']=$allowDoubles;
$config['startTime']=$timeSongStart;
$config['duration']=$timeSongEnd-$timeSongStart;

//write to config.ini
$fp=fopen($configFile,'w');
foreach($config as $key=>$value){
	$value='\''.$value.'\'';
	fwrite($fp,$key.'='.$value.PHP_EOL);
}
fclose($fp);

//then create state file to start playing and launch the py script to sync data to ino
$stateFile='state/'.$id.'.state';
$fp=fopen($stateFile,'w');
fwrite($fp,1);	
fclose($fp);

//launch py script
exec('python3 sync.py '.$id.' > /dev/null 2>/dev/null &');


//redirect back to index
header('Location: index.php');