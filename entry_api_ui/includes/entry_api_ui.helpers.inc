<?php
/**
 * @file
 * Helper functions for entry api UI.
 */

function entry_api_ui_get_category_options($section) {


  // Categories are currently hardcoded, untill there is a new service for it.
  $categories = array();

  $categories['Kijken en luisteren'] = array (
    '0.52.0.0.0' => 'Circus',
    '0.50.4.0.0' => 'Concert',
    '0.54.0.0.0' => 'Dansvoorstelling',
    '0.5.0.0.0' => 'Festival',
    '0.50.6.0.0' => 'Film',
    '0.3.2.0.0' => 'Lezing of gesprek',
    '0.55.0.0.0' => 'Theatervoorstelling',
    '0.56.0.0.0' => 'Toeristisch evenement',
    '0.19.0.0.0' => 'Wedstrijd bekijken',
  );

  $categories['Doen'] = array (
    '0.7.0.0.0' => 'Begeleide uitstap of rondleiding',
    '0.3.3.0.0' => 'Congres of studiedag',
    '0.3.1.0.0' => 'Cursus of workshop',
    '1.50.0.0.0' => 'Eten en drinken',
    '0.57.0.0.0' => 'Kamp of vakantie',
    '0.28.0.0.0' => 'Kermis of feestelijkheid',
    '0.49.0.0.0' => 'Party of fuif',
    '0.53.0.0.0' => 'Recreatiedomein of ûcentrum',
    '0.17.0.0.0' => 'Route',
    '0.50.21.0.0' => 'Spel of quiz',
    '0.41.0.0.0' => 'Thema- of pretpark',
    '0.59.0.0.0' => 'Wedstrijd meedoen',
  );

  $categories['Bezoeken'] = array (
    '0.6.0.0.0' => 'Beurs',
    '0.15.0.0.0' => 'Landschap of natuurgebied',
    '0.37.0.0.0' => 'Markt of braderie',
    '0.14.0.0.0' => 'Monument',
    '0.12.0.0.0' => 'Opendeurdag',
    '0.16.0.0.0' => 'Park of tuin',
    '0.0.0.0.0' => 'Tentoonstelling',
  );

  return isset($categories[$section]) ? $categories[$section] : array();

}