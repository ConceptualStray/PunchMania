<?php

//check in /state folder if we have a file named <id>.state
//if we have, we are playing

//scan the dir
$states=scandir('state');
foreach($states as $state){
	if($state=='.' || $state=='..') continue;
	//check if the file is a state file
	if(preg_match('/\d+\.state/',$state)){
		//get the id from the file
		$__id=explode('.',$state)[0];
		header('Location: control/playing.php?id='.$__id);
		exit;
	}
}

header('Location: control/picker.php');
exit;

