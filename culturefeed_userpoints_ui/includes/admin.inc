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
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_minimum', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
  
  $form['culturefeed_userpoints_ui_exchange_list'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for a list view'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_list', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );
  
  $form['culturefeed_userpoints_ui_exchange_detail'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for a detail view'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_detail', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );
 
  $form['culturefeed_userpoints_ui_exchange_like'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for liking an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_like', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for mailing an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_mail', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_print'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for printing an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_print', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for sharing an item on Facebook'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_facebook', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for sharing an item on Twitter'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_twitter', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
      
  $form['culturefeed_userpoints_ui_exchange_go'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for referring to an item as "I go"'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_go', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
      
  $form['culturefeed_userpoints_ui_exchange_ticket'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for consulting ticketing'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_ticket', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
    
  $form['culturefeed_userpoints_ui_exchange_route'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for consulting a route of an item'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_route', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
    
  $form['culturefeed_userpoints_ui_exchange_more_info'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for consulting more information'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_more_info', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
  
  $form['culturefeed_userpoints_ui_exchange_checkin'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for checking in'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_checkin', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );   
  
  $form['culturefeed_userpoints_ui_exchange_comment'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for posting a comment'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_comment', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );      
  
  $form['culturefeed_userpoints_ui_exchange_recommend'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for recommending an activity'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_recommend', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );
  
  $form['culturefeed_userpoints_ui_exchange_recommend_review'] = array(
    '#type' => 'textfield',
    '#title' => t('Points when recommending a review'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_recommend_review', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );         
  
  $form['culturefeed_userpoints_ui_exchange_connect_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for connecting to a channel'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_connect_channel', '100'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
  
  $form['culturefeed_userpoints_ui_exchange_review'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for posting a review'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_review', '100'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );     
  
  $form['culturefeed_userpoints_ui_exchange_media_photo'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for uploading a photo'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_media_photo', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );     
    
  $form['culturefeed_userpoints_ui_exchange_media_video'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for uploading a video'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_media_video', '0'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );    
  
  $form['culturefeed_userpoints_ui_exchange_page_created'] = array(
    '#type' => 'textfield',
    '#title' => t('Points for creating a page'),
    '#default_value' => variable_get('culturefeed_userpoints_ui_exchange_page_created', '100'),
    '#description' => t('This value is only used as a variable for text, labels, ... . Points management is managed through the UiTiD admin and the corresponding channel / service consumer. Visit www.uitid.be/uitid/rest/admin."'),  
  );  
   
  return system_settings_form($form);

}