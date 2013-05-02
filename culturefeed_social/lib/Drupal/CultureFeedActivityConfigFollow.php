<?php

class CultureFeedActivityConfigFollow extends CultureFeedActivityConfigBase {
  
  public $type = CultureFeed_Activity::TYPE_FOLLOW;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('event');
    
    $this->subject = 'Volgen deze pagina';
    $this->subjectUndo = 'Volg ik niet meer';
    $this->titleDo = 'Ik volg deze pagina';
    $this->titleDoFirst = 'Volg ik als eerste';
    $this->titleShowAll = 'Toon iedereen die deze pagina volgt';
    $this->linkClassDo = 'follow-link';
    $this->linkClassUndo = 'unfollow-link';
  }
  
}
