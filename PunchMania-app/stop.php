<?php

//detele the state file
$id=$_GET['id'];
unlink('state/'.$id.'.state');


//redirect to index
header('Location: index.php');