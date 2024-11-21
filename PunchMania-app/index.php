<?php

require_once 'includes/template.php';
require_once 'includes/material.php';
require_once 'includes/templateFromFile.php';

$page=new material('test');

$url = 'http://192.168.1.20:8000/start.php'; 
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, false); 
curl_setopt($ch, CURLOPT_TIMEOUT, 1); 
curl_exec($ch);
curl_close($ch);

$page->setBody('no response');
echo $page;