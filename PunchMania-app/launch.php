<?php
$id=$_GET['id'];
$command = escapeshellcmd('py exec.py '.$id);
$output = shell_exec($command);