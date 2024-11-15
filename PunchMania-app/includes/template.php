<?php

abstract class template{
	
	protected $string;
	protected $values=[];
	protected $dateFormat;

	protected  function error(string $msg):void{
		throw new ErrorException($msg);
	}

	public function reset():void{
		$this->values=[];
	}

	protected function dateFormat(?string $customDateFormat):void{
		if(!$customDateFormat){
			$this->dateFormat='Y-m-d';
		}else{
			$this->dateFormat=$customDateFormat;
		}
	}

	protected function setArr($dataArr,?string $customDateFormat=NULL):void{
		$this->dateFormat($customDateFormat);
		if($dataArr){
			foreach($dataArr as $key=>$val){
				if($val instanceof DateTime){
					$val=$val->format($this->dateFormat);
				}
				$this->set($key,$val);
			}
		}
	}

	protected function set(string $key,?string $value):void {
		$this->values['[@'.$key.']']=$value;
	}

	protected function appendTo(string $key,?string $value):void{
		if(isset($this->values['[@'.$key.']'])){
			$this->values['[@'.$key.']'].=$value;
		}else{
			$this->values['[@'.$key.']']=$value;
		}
		
	}

	protected function draw():string{
		$string = $this->string;
		$output=str_replace(array_keys($this->values), $this->values, $string);
		return $output;
	}

	public function __toString(){
		return $this->draw();
	}

}
