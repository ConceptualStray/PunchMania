<?php
$id = 1; // Example ID
$command = escapeshellcmd('python play.py '.$id);
$output = shell_exec($command);
echo $output;