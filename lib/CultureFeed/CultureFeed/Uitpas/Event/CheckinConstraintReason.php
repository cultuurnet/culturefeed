<?php
/**
 * @file
 */

class CultureFeed_Uitpas_Event_CheckinConstraintReason {

  // @todo Is this really a checkin constraint reason?
  const ACTION_FAILED = 'ACTION_FAILED';

  const INVALID_CARD = 'INVALID_CARD';

  const INVALID_CARD_STATUS = 'INVALID_CARD_STATUS';

  const INVALID_DATE_TIME = 'INVALID_DATE_TIME';

  const KANSENSTATUUT_EXPIRED = 'KANSENSTATUUT_EXPIRED';

  const MAXIMUM_REACHED = 'MAXIMUM_REACHED';



  static public function all() {
    return array(
      self::ACTION_FAILED,
      self::INVALID_CARD,
      self::INVALID_CARD_STATUS,
      self::INVALID_DATE_TIME,
      self::KANSENSTATUUT_EXPIRED,
      self::MAXIMUM_REACHED,
    );
  }
}
