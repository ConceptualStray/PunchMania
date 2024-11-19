<?php

require_once 'includes/template.php';
require_once 'includes/material.php';
require_once 'includes/templateFromFile.php';

$page=new material();
$body=new templateFromFile('page/index.html');


$caroItems=[];
$songsDir=scandir('data');
foreach($songsDir as $id){
	if($id=='.' || $id=='..') continue;
	$config=parse_ini_file('data/'.$id.'/config.ini');
	$item=new templateFromFile('page/caro-item.html');
	$rid=rand(0,1000000);
	$item->setArr([
		'rnd'=>'rid'.$rid,
		'id'=>$id,
		'img'=>'data/'.$id.'/cover.png',
		'sName'=>$config['name']??'Unknown on ID:'.$id,
		'sAlbum'=>$config['album']??'Unknown on ID:'.$id,
	]);
	$caroItems[]=$item->draw();
}


$body->set('caroItems',join('',$caroItems));


$page->setTitle('PunchMania');
$page->setBody($body);
$page->setToFooter([
	'<script src="page/index.js"></script>',
	'<script src="https://cdn.jsdelivr.net/gh/maxshuty/accessible-web-components@latest/dist/simpleRange.min.js"></script>
']);
$page->setToHeader([
	'<link rel="stylesheet" href="page/index.css">',
	'<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />'
]);
echo $page;