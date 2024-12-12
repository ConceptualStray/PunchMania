<?php

$states = scandir('../state');
$status = false;

foreach ($states as $state) {
    if ($state == '.' || $state == '..') continue;
    if (preg_match('/\d+\.state/', $state)) {
        $status = true;
        break;
    }
}

echo $status ? 'true' : 'false';

