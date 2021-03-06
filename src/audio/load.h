#ifndef AUDIO_LOAD_H
#define AUDIO_LOAD_H

#include "internal.h"

#define AUDIO_FRAME_DMA_QUEUE_SIZE 0x40

#define PRELOAD_BANKS 2
#define PRELOAD_SEQUENCE 1

#define IS_SEQUENCE_CHANNEL_VALID(ptr) ((uintptr_t)(ptr) != (uintptr_t)&DUMMYSUB)

extern struct Note* gNotes;

// Music in SM64 is played using 3 players:
// GROUP_TRACK[0] is level background music
// GROUP_TRACK[1] is misc music, like the puzzle jingle
// GROUP_TRACK[2] is sound
extern struct SequencePlayer GROUP_TRACK[SEQUENCE_PLAYERS];

extern struct SequenceChannel gSequenceChannels[32];

extern struct SequenceChannelLayer gSequenceLayers[52];

extern struct SequenceChannel DUMMYSUB;

extern struct AudioListItem gLayerFreeList;
extern struct NotePool gNoteFreeLists;

extern OSMesgQueue gCurrAudioFrameDmaQueue;
extern u32 gSampleDmaNumListItems;
extern ALSeqFile* gAlTbl;
extern u8* gAlBankSets;

void audio_dma_partial_copy_async(uintptr_t* devAddr, u8** vAddr, ssize_t* remaining, OSMesgQueue* queue, OSIoMesg* mesg);
void decrease_sample_dma_ttls(void);
void* dma_sample_data(uintptr_t devAddr, u32 size, s32 arg2, u8* arg3);
void init_sample_dma_buffers(s32 arg0);
void patch_audio_bank(struct AudioBank* mem, u8* offset, u32 numInstruments, u32 numDrums);
void preload_sequence(u32 seqId, u8 preloadMask);
void load_sequence(u32 player, u32 seqId, s32 loadAsync);

#endif /* AUDIO_LOAD_H */
