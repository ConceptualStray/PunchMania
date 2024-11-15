<?php


class configFile{
	private $file;
	public function __construct(int $id){
		$this->file=parse_ini_file(__DIR__.'/../data/'.$id.'/config.ini');
	}

	private function get(string $key){
		if(isset($this->file[$key])){
			return $this->file[$key];
		}else{
			return NULL;
		}
	}

	private function set(string $key, $value){
		$this->file[$key]=$value;
	}


	public function getName():string{
		return $this->get('name');
	}

	public function setName(string $name):void{
		$this->set('name',$name);
	}

	public function getDesc():string{
		return $this->get('desc');
	}

	public function setDesc(string $desc):void{
		$this->set('desc',$desc);
	}

	public function getArtist():string{
		return $this->get('artist');
	}

	public function setArtist(string $author):void{
		$this->set('artist',$author);
	}

	public function getAlbum():string{
		return $this->get('album');
	}

	public function setAlbum(string $album):void{
		$this->set('album',$album);
	}

	public function getStartTime():int{
		return $this->get('startTime');
	}

	public function setStartTime(int $startTime):void{
		$this->set('startTime',$startTime);
	}

	public function getEndTime():int{
		return $this->get('endTime');
	}

	public function setEndTime(int $endTime):void{
		$this->set('endTime',$endTime);
	}

	public function allowDoubles(?bool $allowDoubles=true):bool{
		$this->set('allowDoubles',$allowDoubles);
		return $this->get('allowDoubles');
	}

	public function setOffset(int $offset):void{
		$this->set('offset',$offset);
	}

	public function getOffset():int{
		return $this->get('offset');
	}

	public function scanAhead(int $scanAhead):int{
		$this->set('scanAhead',$scanAhead);
		return $this->get('scanAhead');
	}



}