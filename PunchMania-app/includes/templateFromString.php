<?php


class templateFromString extends template{

	public function __construct(string $string){
		$this->string=$string;
	}

	public function append(string $string){
		$this->string.=$string;
	}

	public function setArr($dataArr,?string $customDateFormat=NULL):void{
		parent::setArr($dataArr,$customDateFormat);
	}

	public function set(string $key,?string $value):void {
		parent::set($key,$value);
	}

	public function draw():string{
		return parent::draw();
	}

	public function dateFormat(?string $customDateFormat):void{
		parent::dateFormat($customDateFormat);
	}
}
