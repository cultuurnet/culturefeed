<?php

/**
 * Helper functions for culturefeed_saved_searches.
 */

/**
 * Get the frequency options for saved searches.
 */
function culturefeed_saved_searches_frequency_options() {

  return array(
    CultureFeed_SavedSearches_SavedSearch::ASAP => t('immediately'),
    CultureFeed_SavedSearches_SavedSearch::DAILY => t('daily overview'),
    CultureFeed_SavedSearches_SavedSearch::WEEKLY => t('weekly overview'),
    CultureFeed_SavedSearches_SavedSearch::NEVER => t('never'),
  );

}
