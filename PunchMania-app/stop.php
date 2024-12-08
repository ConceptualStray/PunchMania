<?php

//detele the state file
$id=$_GET['id'];
unlink('state/'.$id.'.state');
exec('taskkill /F /IM python.exe');

$pyWorker=popen('start /B py stop.py', "r");
pclose($pyWorker);

//redirect to index
header('Location: index.php');