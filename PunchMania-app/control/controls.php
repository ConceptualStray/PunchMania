<?php
require_once '../includes/template.php';
require_once '../includes/templateFromFile.php';


$id=$_GET['id'];
$config=parse_ini_file('../data/'.$id.'/config.ini');

$notes=file('../data/'.$id.'/notes.ini', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);


//calculate bpm from notes , timestamps are in ms
$totalTime=end($notes);
$totalCount=count($notes);


//calculate bpm
$bpm=round($totalCount/($totalTime/60000),2);


$body=new templateFromFile('/page/controls.html',true);


$body->setArr($config);

$endTime=($config['duration']!=0)?$config['startTime']+$config['duration']:$totalTime;

//read file from points/{id}.score file
$points=0;
if(file_exists('../points/'.$id.'.score')){
	$points=file_get_contents('../points/'.$id.'.score');
}

$allowDoubles='';
$flipSides='';
$skipBeats='';
if($config['flipSides'])$flipSides='checked="checked"';
if($config['allowDoubles']??false)$allowDoubles='checked="checked"';
if($config['skipBeats']??false)$skipBeats='checked="checked"';


$body->set('doubles',$allowDoubles);
$body->set('flipSides',$flipSides);
$body->set('skipBeats',$skipBeats);
$body->set('bpm',$bpm);
$body->set('tNotes',$totalCount);
$body->set('endTime',$endTime/1000);
$body->set('totalTimeInt',$totalTime/1000);
$body->set('id',$id);
$body->set('points',$points);

//convert totalTime in seconds to minutes:seconds
$totalTime=round($totalTime/1000);
$minutes=floor($totalTime/60);
$seconds=$totalTime%60;
$body->set('totalTime',$minutes.':'.str_pad($seconds,2,'0',STR_PAD_LEFT));

echo $body;