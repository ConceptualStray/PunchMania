<?php

require_once 'includes/template.php';
require_once 'includes/material.php';
require_once 'includes/templateFromFile.php';

//just redefine it here so IDE won't complain
$id=$__id;


$page=new material();
$page->setTitle('PunchMania');
$page->setToHeader([
	'<link rel="stylesheet" href="page/index.css">',
	'<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />'
]);
$page->setToFooter([
	'<script src="page/index.js"></script>',
	'<script src="https://cdn.jsdelivr.net/gh/maxshuty/accessible-web-components@latest/dist/simpleRange.min.js"></script>
']);

$body=new templateFromFile('page/playing.html');

$config=parse_ini_file('data/'.$id.'/config.ini');
$body->setArr([
	'sTitle'=>$config['name']??'Unknown on ID:'.$id,
	'sAlbum'=>$config['album']??'Unknown on ID:'.$id,
	'img'=>'data/'.$id.'/cover.png',
	'id'=>$id,
]);






$page->setBody($body);
echo $page;