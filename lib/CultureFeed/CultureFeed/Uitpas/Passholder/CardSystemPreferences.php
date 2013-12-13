<?php

class CultureFeed_Uitpas_Passholder_CardSystemPreferences extends CultureFeed_Uitpas_ValueObject {

  const EMAIL_NO_MAILS = 'NO_MAILS';
  const EMAIL_NOTIFICATION_MAILS = 'NOTIFICATION_MAILS';
  const EMAIL_ALL_MAILS = 'ALL_MAILS';

  const SMS_NO_SMS = 'NO_SMS';
  const SMS_ALL_SMS = 'ALL_SMS';
  const SMS_NOTIFICATION_SMS = 'NOTIFICATION_SMS';

  /**
   * ID of the user.
   *
   * @var string
   */
  public $id;

  /**
   * ID of the advantage object.
   *
   * @var int
   */
  public $cardSystemId;

  /**
   * The e-mail preference
   *
   * @var string
   */
  public $emailPreference;

  /**
   * The SMS preference
   *
   * @var string
   */
  public $smsPreference;

  /**
   * End date kansenstatuut.
   *
   * @var integer
   */
  public $kansenStatuutEndDate;

  /**
   * The consumer key of the counter from where the request originates
   *
   * @var string
   */
  public $balieConsumerKey;

  protected function manipulatePostData(&$data) {

    if (isset($data['kansenStatuutEndDate'])) {
      $data['kansenStatuutEndDate'] = date('Y-m-d', $data['kansenStatuutEndDate']);
    }

  }

  public static function createFromXML(CultureFeed_SimpleXMLElement $object) {

    $cardsystemPreferences = new CultureFeed_Uitpas_Passholder_CardSystemPreferences();
    $cardsystemPreferences->id = $object->xpath_str('id');
    $cardsystemPreferences->cardSystemId = $object->xpath_int('cardSystemId');
    $cardsystemPreferences->emailPreference = $object->xpath_str('emailPreference');
    $cardsystemPreferences->smsPreference = $object->xpath_str('smsPreference');
    $cardsystemPreferences->kansenStatuutEndDate = $object->xpath_time('kansenStatuutEndDate');

    return $cardsystemPreferences;
  }

}
