#include <iostream>
enum PLAYER_RANK
{
    PR_IRON,
    PR_BRONZE,
    PR_SILVER,
    PR_GOLD,
    PR_PLATINUM,
    PR_DIAMOND
    };
enum MEMBERSHIP_TIER
{
    MT_GOLD,
    MT_SILVER,
    MT_FREE,
    MT_BANNED
    };
void DisplaySizeChecks();
void PrintMembership(MEMBERSHIP_TIER membershipTier);
MEMBERSHIP_TIER UpgradeMembershipOneTier(MEMBERSHIP_TIER current);
void PrintPlayerRank(PLAYER_RANK playerRank);
void PromotePlayer(PLAYER_RANK& playerRank);
void DemotePlayer(PLAYER_RANK& playerRank);
void UseEnumerations()
{
    MEMBERSHIP_TIER exampleMembership = MT_FREE;
    PrintMembership(exampleMembership);
    exampleMembership = UpgradeMembershipOneTier(exampleMembership);
    PrintMembership(exampleMembership);

    // initial rank is bronze
    PLAYER_RANK playerRank = PR_BRONZE;
    PrintPlayerRank(playerRank);

    // promote 3 times and demote once
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);
    PromotePlayer(playerRank);

    // gold rank expected
    PrintPlayerRank(playerRank);

    DisplaySizeChecks();
}

void DisplaySizeChecks()
{
    std::cout << "sizeof(PLAYER_RANK) is " << sizeof(PLAYER_RANK);
    std::cout << " bytes." << std::endl;
    std::cout << "sizeof(MEMBERSHIP_STATUS) is ";
    std::cout << sizeof(MEMBERSHIP_TIER) << " bytes." << std::endl;
}


MEMBERSHIP_TIER UpgradeMembershipOneTier(MEMBERSHIP_TIER current)
{
    if (current > MT_GOLD)
    {
        current = static_cast<MEMBERSHIP_TIER>(current - 1);
    }
    return current;
}

void PromotePlayer(PLAYER_RANK &playerRank) {
    // increase rank only if below max rank
    if (playerRank < PR_DIAMOND) {
        playerRank = static_cast<PLAYER_RANK>(playerRank + 1);
    }
}

void DemotePlayer(PLAYER_RANK &playerRank) {
    // decrease only if above min rank
    if (playerRank > PR_IRON) {
        playerRank = static_cast<PLAYER_RANK>(playerRank - 1);
    }
}


void PrintPlayerRank(PLAYER_RANK playerRank) {
    switch (playerRank)
    {
        case PR_IRON:
            std::cout << "Playerrank is iron level!" << std::endl;
            break;
        case PR_BRONZE:
            std::cout << "Playerrank is bronze level." << std::endl;
            break;
        case PR_SILVER:
            std::cout << "Playerrank is silver level." << std::endl;
            break;
        case PR_GOLD:
            std::cout << "Playerrank is gold level." << std::endl;
            break;
        case PR_PLATINUM:
            std::cout << "Playerrank is platinum level." << std::endl;
            break;
        case PR_DIAMOND:
            std::cout << "Playerrank is diamond level." << std::endl;
            break;

        default:
            std::cout << "Unknown membership!" << std::endl;
            break;
    }
}



void PrintMembership(MEMBERSHIP_TIER membershipTier)
{
    switch (membershipTier)
    {
        case MT_GOLD:
            std::cout << "Member is gold level!" << std::endl;
            break;
        case MT_SILVER:
            std::cout << "Member is silver level." << std::endl;
            break;
        case MT_FREE:
            std::cout << "Member is freemium." << std::endl;
            break;
        case MT_BANNED:
            std::cout << "Member is banned!" << std::endl;
            break;
        default:
            std::cout << "Unknown membership!" << std::endl;
            break;
    }
}
