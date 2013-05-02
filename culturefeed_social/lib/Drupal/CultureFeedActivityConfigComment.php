<?php

class CultureFeedActivityConfigComment extends CultureFeedActivityConfigBase {

  public $type = CultureFeed_Activity::TYPE_COMMENT;

  /**
   * Constructor to load default values.
   */
  public function __construct() {
    parent::__construct();

    $this->allowedTypes = array('event', 'book');

    $this->subject = 'Reactie door';
    $this->subjectUndo = '';
    $this->titleDo = 'Ook gereageerd';
    $this->titleDoFirst = 'Reageer als eerste';
    $this->titleShowAll = 'Toon iedereen reageerde op deze pagina';
    $this->linkClassDo = 'comment-link';
    $this->linkClassUndo = 'uncomment-link';
  }

}