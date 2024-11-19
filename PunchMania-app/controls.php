<?php
require_once 'includes/template.php';
require_once 'includes/templateFromFile.php';


$id=$_GET['id'];
$config=parse_ini_file('data/'.$id.'/config.ini');

$notes=file('data/'.$id.'/notes.ini', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

//calculate bpm from notes , timestamps are in ms
//sum all timestamps from $notes
$totalTime=0;
$totalCount=count($notes);
array_walk($notes,function($note) use (&$totalTime){
	$totalTime+=$note;
});

//calculate bpm
$bpm=round($totalCount/($totalTime/60000),2);


$body=new templateFromFile('/page/controls.html',true);
$body->setArr($config);

$endTime=($config['duration']!=0)?$config['startTime']+$config['duration']:$totalTime;
$body->set('bpm',$bpm);
$body->set('tNotes',$totalCount);
$body->set('endTime',$endTime/1000);
$body->set('totalTimeInt',$totalTime/1000);

//convert totalTime in seconds to minutes:seconds
$totalTime=round($totalTime/1000);
$minutes=floor($totalTime/60);
$seconds=$totalTime%60;
$body->set('totalTime',$minutes.':'.str_pad($seconds,2,'0',STR_PAD_LEFT));

echo $body;