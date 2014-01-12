<?php
/**
 * @file
 */ 

class CultureFeed_Uitpas_Passholder_Query_RegisterInCardSystemOptions extends CultureFeed_Uitpas_ValueObject {

  /**
   * @var string
   */
  public $uitpasNumber;

  /**
   * @var int
   */
  public $cardSystemId;

  /**
   * @var bool
   */
  public $kansenStatuut;

  /**
   * @var string
   */
  public $kansenStatuutEndDate;

  /**
   * @var string
   */
  public $balieConsumerKey;

  // @todo Add other properties.

  /**
   * {@inheritdoc}
   */
  protected function manipulatePostData(&$data) {
    if (isset($data['kansenStatuut'])) {
      $data['kansenStatuut'] = $data['kansenStatuut'] ? 'true' : 'false';
    }

    if (isset($data['kansenStatuutEndDate'])) {
      $data['kansenStatuutEndDate'] = date('Y-m-d', $data['kansenStatuutEndDate']);
    }
  }

}
