package chat.serialization.packets;

public class InitialPacket {
    private String name;
    private NameStatus nameStatus;

    public enum NameStatus {
        NAME_CREATED,
        NAME_EMPTY,
        NAME_ALREADY_PRESENTED
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public NameStatus getNameStatus() {
        return nameStatus;
    }

    public void setNameStatus(NameStatus nameStatus) {
        this.nameStatus = nameStatus;
    }
}

