<?php

/**
 * Generic configuration object for all type of activity.
 */
class CultureFeedActivityConfigBase {

  public $nodeTypes;
  public $allowedTypes;
  public $type;
  public $subject = '';
  public $subjectDo = '';
  public $subjectUndo = '';
  public $titleDo = '';
  public $titleDoFirst = '';
  public $titleShowAll = '';
  public $linkClassDo = '';
  public $linkClassUndo = '';
  
  /**
   * Constructor to load default values.
   */
  public function __construct() {
    $this->nodeTypes = array_keys(node_type_get_types());
    $this->allowedTypes = array('event');
  }
  
  /**
   * Factory method to retrieve the correct CultureFeedActivityConfig object. 
   */
  public static function loadByType($type) {
    
    switch ($type) {
      
      case CultureFeed_Activity::TYPE_RECOMMEND:
        return new CultureFeedActivityConfigRecommend();
        break;
      
      case CultureFeed_Activity::TYPE_LIKE:
        return new CultureFeedActivityConfigLike();
        break;
      
      case CultureFeed_Activity::TYPE_COMMENT:
        return new CultureFeedActivityConfigComment();
        break;
      
      case CultureFeed_Activity::TYPE_FOLLOW:
        return new CultureFeedActivityConfigFollow();
        break;
        
    }
    
  }
  
}
