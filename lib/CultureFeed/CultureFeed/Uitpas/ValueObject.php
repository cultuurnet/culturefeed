<?php

abstract class CultureFeed_Uitpas_ValueObject {

  public function toPostData() {
    $data = get_object_vars($this);
    $this->manipulatePostData($data);
    $data = array_filter($data);

    return $data;
  }

  protected function manipulatePostData($data) {

  }

}