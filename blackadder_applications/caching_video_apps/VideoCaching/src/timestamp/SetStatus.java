package timestamp;

public enum SetStatus
{
	OK_NEXT,              // OK, and it was the next expected value
	OK_STORED,            // OK, stored and we're waiting for the next value
	SKIPPED_FUTURE,       // It was outside capacity, so we skipped some not-yet-received timestamps to store this
	//DROPPED_FUTURE,       // It was outside capacity, so it was rejected
	DROPPED_PAST,         // Rejected, this timestamp is in the past
	OVERWRITE_SAME,       // This timestamp was already stored, and the stored value was the same
	OVERWRITE_DIFFERENT   // This timestamp was already stored, and the stored value was different
};
