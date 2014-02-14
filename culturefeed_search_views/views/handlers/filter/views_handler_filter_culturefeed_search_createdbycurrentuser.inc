<?php
/**
 * @file
 */
/**
 * @class
 * Handler class to handle filtering createdby.
 */
class views_handler_filter_culturefeed_search_createdbycurrentuser extends
  views_handler_filter_culturefeed_search_createdby {

  /**
   * @see views_handler_filter::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->search_field = 'createdby';

  }

  /**
   * @see views_handler_filter_in_operator::value_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'markup',
      '#markup' => t('Created by current user'),
    );
  }

  /**
   * {@inheritdoc}
   *
   * Return an empty description as our value is already contained in the
   * Views filter name.
   */
  function admin_summary()
  {
    return '';
  }

  /**
   * Get the value to query.
   */
  function getValue() {
    if (culturefeed_is_culturefeed_user()) {
      /** @var CultureFeed_User $culturefeed_user */
      $culturefeed_user = DrupalCultureFeed::getLoggedInUser();

      if ($culturefeed_user->mbox) {
        return $culturefeed_user->mbox;
      }
    }

    // Return a string which shouldn't match any createdby value
    // so search does not return anything.
    return '??notavalidnickname??';
  }

}
