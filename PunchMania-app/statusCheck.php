<?php

$states=scandir('state');
foreach($states as $state){
	if($state=='.' || $state=='..') continue;
	//check if the file is a state file
	if(preg_match('/\d+\.state/',$state)){
		//get the id from the file
		$__id=explode('.',$state)[0];
		exit('true');
	}
}

exit('false');