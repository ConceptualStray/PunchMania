<?php
$id = 1; // Example ID
$command = escapeshellcmd('python exec.py '.$id);
$output = shell_exec($command);
echo $output;