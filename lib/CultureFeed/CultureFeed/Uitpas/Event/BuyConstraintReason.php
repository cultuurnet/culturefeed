<?php
/**
 * @file
 */

class CultureFeed_Uitpas_Event_BuyConstraintReason {
  const INVALID_CARD = 'INVALID_CARD';

  const INVALID_CARD_STATUS = 'INVALID_CARD_STATUS';

  const KANSENSTATUUT_EXPIRED = 'KANSENSTATUUT_EXPIRED';

  const MAXIMUM_REACHED = 'MAXIMUM_REACHED';

  const INVALID_DATE_CONSTRAINTS = 'INVALID_DATE_CONSTRAINTS';

  static public function all() {
    return array(
      self::INVALID_CARD,
      self::INVALID_CARD_STATUS,
      self::KANSENSTATUUT_EXPIRED,
      self::MAXIMUM_REACHED,
      self::INVALID_DATE_CONSTRAINTS,
    );
  }
}
