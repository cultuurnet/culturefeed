<?php
/**
 *
 */

class CultureFeed_Uitpas_Passholder_Query_EventActions extends CultureFeed_Uitpas_ValueObject {

  public $uitpasNumber;

  public $chipNumber;

  public $cdbid;

  public $autocheckin;

  public $balieConsumerKey;

  public function manipulatePostData(&$data) {
    if (isset($data['autocheckin'])) {
      $data['autocheckin'] = $data['autocheckin'] ? 'true' : 'false';
    }
  }
}
