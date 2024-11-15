<?php



class timer{

	private $startExecTime;

	public function __construct(){
		$this->startExecTime = microtime(TRUE);
	}

	public function getExecTime():string{
		return microtime(true) - $this->startExecTime;
	}

	public function __toString(){
		return $this->getExecTime();
	}
}