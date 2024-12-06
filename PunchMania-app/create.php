<?php
require_once 'includes/template.php';
require_once 'includes/material.php';
require_once 'includes/templateFromFile.php';
$page=new material();
$page->setTitle('PunchMania');
$page->setToHeader([
	'<link rel="stylesheet" href="page/index.css">',
	'<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />'
]);
$page->setToFooter([
	'<script src="page/create.js"></script>',
	'<script src="https://cdn.jsdelivr.net/gh/maxshuty/accessible-web-components@latest/dist/simpleRange.min.js"></script>
']);


$body=new templateFromFile('page/create.html',true);
$id=isset($_GET['id'])?$_GET['id']:0;

if(!$id)exit('No id');
$body->set('id',$id);

$page->setBody($body);
echo $page;