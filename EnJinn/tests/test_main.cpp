#include <catch2/catch_test_macros.hpp>
#include "../core/enjinnEditor/phaseManager/phaseManager.h"

TEST_CASE("Dummy Test", "[dummy]") {
    REQUIRE(1 == 1);
}

TEST_CASE("PhaseManager Unlock", "[phaseManager]") {
    enjinn::PhaseManager pm;
    REQUIRE_FALSE(pm.IsPhaseSelectorUnlocked());

    // Test incorrect secret
    REQUIRE_FALSE(pm.TryUnlockPhaseSelector("wrong"));
    REQUIRE_FALSE(pm.IsPhaseSelectorUnlocked());

    // Test correct secret
    REQUIRE(pm.TryUnlockPhaseSelector("jayyvarmaa"));
    REQUIRE(pm.IsPhaseSelectorUnlocked());
}
