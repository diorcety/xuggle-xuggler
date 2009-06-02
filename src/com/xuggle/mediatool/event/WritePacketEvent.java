/**
 * 
 */
package com.xuggle.mediatool.event;

import com.xuggle.mediatool.IMediaCoder;
import com.xuggle.xuggler.IPacket;

public class WritePacketEvent extends PacketEvent
{
  public WritePacketEvent(IMediaCoder source, IPacket packet)
  {
    super(source, packet);
  }
}