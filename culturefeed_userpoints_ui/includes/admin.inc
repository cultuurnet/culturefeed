<?php
/**
 * Settings file for culturefeed userpoints ui.
 */

function culturefeed_userpoints_ui_settings_form($form, $form_state = array()) {

  $form['culturefeed_userpoints_ui_exchange_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Exchange ratio'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_ratio'),
    '#description' => t('Exchange ratio to recalculate points for this site. E.g. "0.01 points is worth 1 point on this site."'),
  );

  $form['culturefeed_userpoints_ui_exchange_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination email when an exchange has been done'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_email', ''),
    );
    
  $form['culturefeed_userpoints_ui_exchange_minimum'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum points collected before exchanging'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_minimum', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
  
  $form['culturefeed_userpoints_ui_exchange_list'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for a list view'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_list', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );
  
  $form['culturefeed_userpoints_ui_exchange_detail'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for a detail view'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_detail', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );
 
  $form['culturefeed_userpoints_ui_exchange_like'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for liking an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_like', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for mailing an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_mail', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_print'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for printing an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_print', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for sharing an item on Facebook'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_facebook', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for sharing an item on Twitter'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_twitter', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
      
  $form['culturefeed_userpoints_ui_exchange_go'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for referring to an item as "I go"'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_go', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
      
  $form['culturefeed_userpoints_ui_exchange_ticket'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for consulting ticketing'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_ticket', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
    
  $form['culturefeed_userpoints_ui_exchange_route'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for consulting a route of an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_route', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
    
  $form['culturefeed_userpoints_ui_exchange_more_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for consulting more information'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_more_info', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_checkin'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for checking in'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_checkin', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );   
  
  $form['culturefeed_userpoints_ui_exchange_comment'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for posting a comment'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_comment', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );      
  
  $form['culturefeed_userpoints_ui_exchange_recommend'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for recommending an activity'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_recommend', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );      
  
  $form['culturefeed_userpoints_ui_exchange_connect_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for connecting to a channel'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_connect_channel', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
  
  $form['culturefeed_userpoints_ui_exchange_review'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for posting a review'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_review', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );     
  
  $form['culturefeed_userpoints_ui_exchange_media_photo'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for uploading a photo'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_media_photo', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );     
    
  $form['culturefeed_userpoints_ui_exchange_media_video'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for uploading a video'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_media_video', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
  
  $form['culturefeed_userpoints_ui_exchange_page_created'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for creating a page'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_page_created', ''),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
   
  return system_settings_form($form);

}