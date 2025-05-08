#include "AudioToLipsync/CBAudioToLipsyncSequence.h"

void UCBAudioToLipsyncSequence::SetSequenceLength(const float& Length)
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	SequenceLength = Length;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}