<?php
/**
 * @file
 * Wishlist for a users' userpoints.
 */

/**
 * Class CultureFeedUserpointsWishlist
 */
class CultureFeedUserpointsWishlist {

  /**
   * Get all the points in the wishlist.
   */
  public static function get($promotionId = NULL) {
    
    if (isset($_SESSION['culturefeed_userpoints_wishlist'])) {
      
      if (isset($promotionId)){
        if (isset($_SESSION['culturefeed_userpoints_wishlist'][$promotionId])) {
          return $_SESSION['culturefeed_userpoints_wishlist'][$promotionId];
        }
        return NULL;
      }
      else {
        return $_SESSION['culturefeed_userpoints_wishlist'];
      }
      
    }
    
    return array();
    
  }
  
  /**
   * Get all the ids in the wishlist.
   */
  public static function getIds() {


    if (isset($_SESSION['culturefeed_userpoints_wishlist'])) {
      return array_keys($_SESSION['culturefeed_userpoints_wishlist']);
    }
    
    return array();
    
  }
  
  /**
   * Get all the ids in the wishlist.
   */
  public static function getCounts() {

    $counts = array();
    
    if (isset($_SESSION['culturefeed_userpoints_wishlist'])) {
      foreach ($_SESSION['culturefeed_userpoints_wishlist'] as $promotionId => $data) {
        $counts['promotionCount' . $promotionId] = $data['count'];
      }
    }
    
    return $counts;
    
  }

  /**
   * Get all the items in the wishlist.
   */
  public static function getItems($promotions) {

    $selected = array();

    if (isset($_SESSION['culturefeed_userpoints_wishlist'])) {
      foreach ($promotions as $key => $promotion) {
        $promotionId = $promotion->id;
        if (isset($_SESSION['culturefeed_userpoints_wishlist'][$promotionId])) {
          $selected[$promotionId] = $_SESSION['culturefeed_userpoints_wishlist'][$promotionId];
          $selected[$promotionId]['title'] = $promotion->title;
        }
      }
    }
  
    return $selected;
  
  }
  
  /**
   * Get all the points in the wishlist.
   */
  public static function getPointsInWishlist() {
    
    $points = 0;
    
    if (isset($_SESSION['culturefeed_userpoints_wishlist'])) {
      foreach ($_SESSION['culturefeed_userpoints_wishlist'] as $promotionId => $item) {
        $points += $item['count'] * $item['points'];
      }
    }
    
    return $points;
    
  }
  
  /**
   * Get the points left for this user.
   * @param unknown $userPoints
   */
  public static function getPointsLeft($userPoints) {
    $userPoints = self::getRealPoints($userPoints);
    $wishlistPoints = self::getRealPoints(self::getPointsInWishlist());
    return $userPoints - $wishlistPoints;
  }

  /**
   * @param Integer $promotionId
   */
  public static function add($promotionId, $promotionCount, $promotion) {

    if (!isset($_SESSION['culturefeed_userpoints_wishlist'][$promotionId])) {
      $_SESSION['culturefeed_userpoints_wishlist'][$promotionId] = array(
        'count' => 0,
        'points' => $promotion->points,
        'promotion_id' => $promotionId,
      );
    }
    
    $_SESSION['culturefeed_userpoints_wishlist'][$promotionId]['count'] = $promotionCount;
    
  }
  
  /**
   * @param Integer $promotionId
   */
  public static function remove($promotionId) {
    
    if (isset($_SESSION['culturefeed_userpoints_wishlist'][$promotionId])) {
      unset($_SESSION['culturefeed_userpoints_wishlist'][$promotionId]);
    }
    
  }
  
  /**
   * @param Integer $promotionId
   */
  public static function has($promotionId) {
    
    return isset($_SESSION['culturefeed_userpoints_wishlist'], $_SESSION['culturefeed_userpoints_wishlist'][$promotionId]);
  
  }
  
  /**
   * Clears the wishlist.
   */
  public static function clear() {
    unset($_SESSION['culturefeed_userpoints_wishlist']);
  }

  /**
   * Calculate the real points for this site.
   * @param Integer $points
   * @return number
   */
  public static function getRealPoints($points) {
  
    $exchange = variable_get('culturefeed_userpoints_ui_exchange_ratio');
    if (!empty($exchange) && is_numeric($exchange)) {
      $points = round($points / $exchange);
    }
  
    return $points;
  
  }
  
}
