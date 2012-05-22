<?php

function uitpas_ui_block_view_filters_where() {
  $locations = array_map('trim', explode("\n", variable_get_value('uitpas_ui_locations')));
}